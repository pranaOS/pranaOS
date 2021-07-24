/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/Format.h>
#include <base/LexicalPath.h>
#include <base/QuickSort.h>
#include <base/Result.h>
#include <base/String.h>
#include <base/StringView.h>
#include <base/Vector.h>
#include <libcore/ConfigFile.h>
#include <libcore/File.h>
#include <spawn.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

enum class ComponentCategory {
    Optional,
    Recommended,
    Required
};

struct ComponentData {
    String name;
    String description;
    ComponentCategory category { ComponentCategory::Optional };
    bool was_selected { false };
    Vector<String> dependencies;
    bool is_selected { false };
};

struct WhiptailOption {
    String tag;
    String name;
    String description;
    bool checked { false };
};

enum class WhiptailMode {
    Menu,
    Checklist
};

static Optional<String> get_current_working_directory()
{
    char* cwd = getcwd(nullptr, 0);
    if (!cwd) {
        perror("getcwd");
        return {};
    }
    String data { cwd };
    free(cwd);
    return data;
}

static Vector<ComponentData> read_component_data(Core::ConfigFile const& config_file)
{
    VERIFY(!config_file.read_entry("Global", "build_everything", {}).is_empty());
    Vector<ComponentData> components;

    auto groups = config_file.groups();
    quick_sort(groups, [](auto& a, auto& b) {
        return a.to_lowercase() < b.to_lowercase();
    });

    for (auto& component_name : groups) {
        if (component_name == "Global")
            continue;
        auto description = config_file.read_entry(component_name, "description", "");
        auto recommended = config_file.read_bool_entry(component_name, "recommended", false);
        auto required = config_file.read_bool_entry(component_name, "required", false);
        auto user_selected = config_file.read_bool_entry(component_name, "user_selected", false);
        auto depends = config_file.read_entry(component_name, "depends", "").split(';');
        VERIFY(!recommended || !required);
        ComponentCategory category { ComponentCategory::Optional };
        if (recommended)
            category = ComponentCategory::Recommended;
        else if (required)
            category = ComponentCategory ::Required;

        components.append(ComponentData { component_name, move(description), category, user_selected, move(depends), false });
    }

    return components;
}

static Result<Vector<String>, int> run_whiptail(WhiptailMode mode, Vector<WhiptailOption> const& options, StringView const& title, StringView const& description)
{
    struct winsize w;
    if (ioctl(0, TIOCGWINSZ, &w) < 0) {
        perror("ioctl");
        return -errno;
    }

    auto height = w.ws_row - 6;
    auto width = min(w.ws_col - 6, 80);

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipefd");
        return -errno;
    }

    int read_fd = pipefd[0];
    int write_fd = pipefd[1];

    Vector<String> arguments = { "whiptail", "--notags", "--separate-output", "--output-fd", String::number(write_fd) };

    if (!title.is_empty()) {
        arguments.append("--title");
        arguments.append(title);
    }

    switch (mode) {
    case WhiptailMode::Menu:
        arguments.append("--menu");
        break;
    case WhiptailMode::Checklist:
        arguments.append("--checklist");
        break;
    default:
        VERIFY_NOT_REACHED();
    }

    if (description.is_empty())
        arguments.append(String::empty());
    else
        arguments.append(String::formatted("\n {}", description));

    arguments.append(String::number(height));
    arguments.append(String::number(width));
    arguments.append(String::number(height - 9));

    // Check how wide the name field needs to be.
    size_t max_name_width = 0;
    for (auto& option : options) {
        if (option.name.length() > max_name_width)
            max_name_width = option.name.length();
    }

    for (auto& option : options) {
        arguments.append(option.tag);
        arguments.append(String::formatted("{:{2}}    {}", option.name, option.description, max_name_width));
        if (mode == WhiptailMode::Checklist)
            arguments.append(option.checked ? "1" : "0");
    }

    char* argv[arguments.size() + 1];
    for (size_t i = 0; i < arguments.size(); ++i)
        argv[i] = const_cast<char*>(arguments[i].characters());
    argv[arguments.size()] = nullptr;

    auto* term_variable = getenv("TERM");
    if (!term_variable) {
        warnln("getenv: TERM variable not set.");
        close(write_fd);
        close(read_fd);
        return -1;
    }

    auto full_term_variable = String::formatted("TERM={}", term_variable);
    auto colors = "NEWT_COLORS=root=,black\ncheckbox=black,lightgray";

    char* env[3];
    env[0] = const_cast<char*>(full_term_variable.characters());
    env[1] = const_cast<char*>(colors);
    env[2] = nullptr;

    pid_t pid;
    if (posix_spawnp(&pid, arguments[0].characters(), nullptr, nullptr, argv, env)) {
        perror("posix_spawnp");
        warnln("\e[31mError:\e[0m Could not execute 'whiptail', maybe it isn't installed.");
        close(write_fd);
        close(read_fd);
        return -errno;
    }

    int status = -1;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        close(write_fd);
        close(read_fd);
        return -errno;
    }

    close(write_fd);

    if (!WIFEXITED(status)) {
        close(read_fd);
        return -1;
    }

    int return_code = WEXITSTATUS(status);
    if (return_code > 0) {
        close(read_fd);
        if (return_code == 127)
            warnln("\e[31mError:\e[0m Could not execute 'whiptail', maybe it isn't installed.");
        return return_code;
    }

    auto file = Core::File::construct();
    file->open(read_fd, Core::OpenMode::ReadOnly, Core::File::ShouldCloseFileDescriptor::Yes);
    auto data = String::copy(file->read_all());
    return data.split('\n', false);
}

static bool run_system_command(String const& command, StringView const& command_name)
{
    if (command.starts_with("cmake"))
        warnln("\e[34mRunning CMake...\e[0m");
    else
        warnln("\e[34mRunning '{}'...\e[0m", command);
    auto rc = system(command.characters());
    if (rc < 0) {
        perror("system");
        warnln("\e[31mError:\e[0m Could not run {}.", command_name);
        return false;
    } else if (rc > 0) {
        warnln("\e[31mError:\e[0m {} returned status code {}.", command_name, rc);
        return false;
    }
    return true;
}

int main()
{
    auto current_working_directory = get_current_working_directory();
    if (!get_current_working_directory.has_value())
        return 1;
    auto lexical_cmd = LexicalPath(*current_working_directory);
    auto& parts = lexical_cmd.parts_view();


    if (!Core::File::exists("components.ini")) {
        warnln("\e[31mError:\e[0m There is no 'components.ini' in the current working directory.");
        warnln("       It can be generated by running CMake with 'cmake ../.. -G Ninja'");
        return 1;
    }

    auto components_file = Core::ConfigFile::open("components.ini");
    if (components_file->groups().is_empty()) {
        warnln("\e[31mError:\e[0m The 'components.ini' file is either not a valid ini file or contains no entries.");
        return 1;
    }


    auto configs_result = run_whiptail(WhiptailMode::Menu, configs, "SerenityOS - System Configurations", "Which system configuration do you want to use or customize?");
    if (configs_result.is_error()) {
        warnln("ConfigureComponents cancelled.");
        return 0;
    }

}