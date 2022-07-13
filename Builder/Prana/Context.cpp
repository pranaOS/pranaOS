#include "Context.h"
#include "Config.h"
#include "Executor/ExecUnit.h"
#include "Executor/Executor.h"
#include "Finder/Finder.h"
#include "Parser/Parser.h"
#include "TimeStampDumper.h"
#include "TimeStampParser.h"
#include "Translator/Translator.h"
#include <numeric>
#include <thread>
#include <utility>
#include <functional>
#include <iostream>

std::unordered_map<std::string, Context*> Context::s_processing_contexts = {};
SpinLock Context::m_lock = {};

static int last_modification_time(const std::filesystem::path& file)
{
    return static_cast<int>(std::filesystem::last_write_time(file).time_since_epoch() / std::chrono::seconds(1));
}

Context::Context(std::filesystem::path path, Context::Operation operation, const DefinesField& defines, bool root_ctx)
    : m_path(std::move(path))
    , m_operation(operation)
    , m_defines(defines)
    , m_root_ctx(root_ctx)
{
    parser = Parser(m_path, this);
}

void Context::run()
{
    m_thread = new std::thread([this]() {
        parser.run();
        validate_fields();
        merge_children();
        process_by_mode();

        m_done = true;
    });
}

bool Context::run_as_childs(const std::string& pattern, Operation operation)
{
    auto prana_files = Finder::FindPranaFiles(directory(), pattern);
    if (prana_files.empty()) {
        return false;
    }
    for (auto& path : prana_files) {
        auto _ = ScopedLocker(m_lock);
        auto context = get_context_by_path(path);
        if (context == nullptr) {
            auto child = new Context(path, operation, m_defines);
            m_children.push_back(child);
            register_context(path, child);
            child->run();
        } else {
            m_children.push_back(context);
        }
    }
    return true;
}

void Context::validate_fields()
{
    if (m_build.type() == BuildField::Type::Executable) {
        if (m_build.archiver()) {
            trigger_error("can\'t use Archiver subfield for Executable type");
        }
    } else if (m_build.type() == BuildField::Type::StaticLib) {
        if (m_build.linker() || !m_build.linker_flags().empty()) {
            trigger_error("can\'t use Link subfield for StaticLib type");
        }
    }
}

bool Context::merge_children()
{
    for (auto child : m_children) {
        if (child->operation() == Context::Operation::Parse) {
            while (child->m_state != Context::State::Parsed) {
                std::this_thread::yield();
            }
        }
    }

    for (auto child : m_children) {
        if (child->operation() == Context::Operation::Parse) {
            for (auto& define : child->m_defines.defines()) {
                m_defines.add_defines(define.first, define.second);
            }

            for (auto& command : child->m_commands.commands()) {
                m_commands.m_commands.emplace(command.first, command.second);
            }
        }
    }

    m_state = State::Parsed;
    return true;
}

void Context::process_by_mode()
{
    if (m_operation != Operation::Build) {
        return;
    }
    auto mode = Config::the().mode();
    const auto& arguments = Config::the().arguments();

    auto process_command = [&](const std::string& cmd) {
        if (cmd == "Build") {
            build();
        } else {
            for (auto& command : m_commands.command_list(cmd)) {
                Executor::blocking_cmd(*command);
            }
        }
    };

    if (mode == Config::Mode::Default) {
        if (m_default.sequence().empty()) {
            process_command("Build");
        }
        for (auto& cmd : m_default.sequence()) {
            process_command(*cmd);
        }
        return;
    }

    if (mode == Config::Mode::CommandList) {
        for (auto& cmd : arguments) {
            process_command(cmd);
        }
        return;
    }

    if (mode == Config::Mode::Generate) {
        Translator::generate_cmake(this);
    }
}

bool Context::build()
{
    fill_timestamps();

    std::vector<std::shared_ptr<std::string>> objects {};

    for (auto& source : m_build.sources()) {
        auto files = Finder::FindFiles(directory(), *source);

        for (auto& file : files) {
            bool recompile_file = false;
            if (scan_include(file) == IncludeStatus::NeedsRecompilation) {
                recompile_file = true;
            }

            m_was_any_recompilation |= recompile_file;

            auto option = m_build.get_option_for_file(file);
            if (!option) {
                trigger_error("no option for file \"" + file.string() + "\"");
            }

            auto object = (prana_path() / std::filesystem::proximate(file, directory())).string() + ".o";
            Finder::CreateDirectory(std::filesystem::path(object).parent_path());

            auto relative_source = std::filesystem::proximate(file, directory());
            auto relative_object = std::filesystem::proximate(object, directory());

            objects.push_back(std::make_shared<std::string>(relative_object));

            if (!recompile_file && std::filesystem::exists(object)) {
                continue;
            }

            auto flags = option->flags;

            if (*option->compiler != "nasm") {
                flags.push_back(std::make_shared<std::string>("-c"));
            }
            flags.push_back(std::make_shared<std::string>(relative_source));
            flags.push_back(std::make_shared<std::string>("-o"));
            flags.push_back(std::make_shared<std::string>(relative_object));

            Executor::the().enqueue(std::make_shared<ExecUnit>(ExecUnit {
                .op = ::Operation::Compile,
                .ctx = this,
                .callee = option->compiler,
                .src = std::move(file),
                .binary = nullptr,
                .args = std::move(flags),
                .cwd = cwd(),
            }));
        }
    }

    while (compile_counter > 0) {
        std::this_thread::yield();
    }

    dump_timestamps();

    auto dependency_libs = std::vector<std::shared_ptr<std::string>>();
    for (auto child : m_children) {
        if (child->operation() == Context::Operation::Build) {
            while (child->m_build.type() == BuildField::Type::Unknown) {
                std::this_thread::yield();
            }
            if (child->m_build.type() == BuildField::Type::StaticLib) {
                auto dependency_lib_relative = std::filesystem::proximate(child->static_library_path(), directory());
                dependency_libs.push_back(std::make_shared<std::string>(dependency_lib_relative));
                while (child->m_state != Context::State::Built) {
                    std::this_thread::yield();
                }
                m_was_any_recompilation |= child->m_was_any_recompilation;
            }
        }
    }

    if (m_state == State::BuildError) {
        exit(1);
    }

    if (m_was_any_recompilation && !objects.empty()) {
        if (m_build.type() == BuildField::Type::StaticLib) {
            size_t lastindex = m_path.string().find_last_of('.');
            std::string libname = m_path.string().substr(0, lastindex);

            auto lib = (prana_path() / std::filesystem::proximate(libname, directory())).string() + ".a";
            auto lib_relative = std::filesystem::proximate(lib, directory());
            auto lib_name = std::make_shared<std::string>(lib_relative);

            auto archiver_flags = std::vector<std::shared_ptr<std::string>>();
            archiver_flags.push_back(std::make_shared<std::string>("rcs"));
            archiver_flags.push_back(lib_name);
            std::copy(objects.begin(), objects.end(), std::back_inserter(archiver_flags));
            std::copy(dependency_libs.begin(), dependency_libs.end(), std::back_inserter(archiver_flags));

            Executor::the().enqueue(std::make_shared<ExecUnit>(ExecUnit {
                .op = ::Operation::Archive,
                .ctx = this,
                .callee = m_build.archiver(),
                .src = {},
                .binary = lib_name,
                .args = std::move(archiver_flags),
                .cwd = cwd() }));
        } else {
            std::copy(dependency_libs.begin(), dependency_libs.end(), std::back_inserter(m_build.linker_flags()));
            std::copy(objects.begin(), objects.end(), std::back_inserter(m_build.linker_flags()));
            std::copy(dependency_libs.begin(), dependency_libs.end(), std::back_inserter(m_build.linker_flags()));
            std::copy(dependency_libs.begin(), dependency_libs.end(), std::back_inserter(m_build.linker_flags()));

            m_build.linker_flags().push_back(std::make_shared<std::string>("-o"));
            auto link_exec = std::make_shared<std::string>(std::filesystem::proximate(executable_path(), directory()));
            m_build.linker_flags().push_back(link_exec);

            Executor::the().enqueue(std::make_shared<ExecUnit>(ExecUnit {
                .op = ::Operation::Link,
                .ctx = this,
                .callee = m_build.linker(),
                .src = {},
                .binary = link_exec,
                .args = std::move(m_build.linker_flags()),
                .cwd = cwd() }));
        }

        while (!done_finalizer) {
            std::this_thread::yield();
        }
    }

    if (m_state == State::BuildError) {
        exit(1);
    }

    for (auto child : m_children) {
        if (child->operation() == Context::Operation::Build) {
            if (child->m_build.type() == BuildField::Type::Executable) {
                while (child->m_state != Context::State::Built) {
                    std::this_thread::yield();
                }
            }
        }
    }

    m_state = State::Built;

    if (root()) {
        Executor::the().stop();
        Executor::the().await();
    }

    return false;
}

IncludeStatus Context::scan_include(const std::filesystem::path& file)
{
    auto path_in_timestamps_file = std::filesystem::proximate(file, directory());

    if (m_path_to_visited_stack_index.contains(path_in_timestamps_file)) {
        std::stringstream error_builder;
        error_builder << "detected a circular dependency starting from \"" + file.string() + "\".\n\nInclude stack:\n\n";
        for (size_t i = m_path_to_visited_stack_index[path_in_timestamps_file] ; i < m_visited_stack.size(); i++) {
            error_builder << m_visited_stack[i] << "\n";
        }
        error_builder << path_in_timestamps_file.string() << "\n";
        trigger_error(error_builder.str());
    }

    if (m_include_status[file] != IncludeStatus::NotVisited) {
        return m_include_status[file];
    }

    auto recursive_include_parser = [&](const std::string& include, bool global) {
        std::filesystem::path include_path;

        if (!global) {
            include_path = file.parent_path() / include;
            if (!std::filesystem::exists(include_path)) {
                trigger_error("can\'t find relative include file \"" + include + "\" in " + file.string());
            }
        } else {
            for (auto& header_folder : m_build.header_folders()) {
                auto path = directory() / *header_folder / include;
                if (std::filesystem::exists(path)) {
                    include_path = path;
                    break;
                }
            }
        }

#if 0
        std::cout << "looking at include: " << include << " from file: " << file << " by path: " << include_path << "\n";
#endif

        if (include_path.empty()) {
            return;
        }

        auto include_status = scan_include(include_path);
        if (include_status == IncludeStatus::NeedsRecompilation) {
            m_include_status[file] = IncludeStatus::NeedsRecompilation;
        }
    };

    m_visited_stack.push_back(path_in_timestamps_file);
    m_path_to_visited_stack_index[path_in_timestamps_file] = m_visited_stack.size() - 1;
    IncludeParser(file).run(recursive_include_parser);
    m_visited_stack.pop_back();
    m_path_to_visited_stack_index.erase(path_in_timestamps_file);

    if (m_include_status[file] == IncludeStatus::NeedsRecompilation) {
        return m_include_status[file];
    }

    if (last_modification_time(file) >= m_timestamps[path_in_timestamps_file]) {
        m_include_status[file] = IncludeStatus::NeedsRecompilation;
    } else {
        m_include_status[file] = IncludeStatus::UpToDate;
    }

    return m_include_status[file];
}

void Context::fill_timestamps()
{
    TimeStampParser(timestamps_path()).run([&](const std::string& path, int timestamp) {
        m_timestamps[path] = timestamp;
    });
}

void Context::dump_timestamps()
{
    auto td = TimeStampDumper(timestamps_path());

    for (auto& [path, status] : m_include_status) {
        if (status == IncludeStatus::NeedsRecompilation) {
            auto path_in_timestamps_file = std::filesystem::proximate(path, directory());
            if (!m_failed_sources.contains(path_in_timestamps_file)) {
                m_timestamps[path_in_timestamps_file] = Config::the().timestamp();
            }
        }
    }

    for (auto& [path, timestamp] : m_timestamps) {
        td.append(path, timestamp);
    }
}