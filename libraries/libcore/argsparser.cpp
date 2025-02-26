/**
 * @file argsparser.cpp
 * @author Krisna Pranav
 * @brief args parser
 * @version 6.0
 * @date 2025-02-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "mods/jsonobject.h"
#include <mods/format.h>
#include <mods/stringbuilder.h>
#include <libcore/argsparser.h>
#include <libcore/version.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * @param s 
 * @return Optional<double> 
 */
static Optional<double> convert_to_double(char const* s)
{
    char* p;
    double v = strtod(s, &p);

    if (isnan(v) || p == s)
        return {};

    return v;
}

namespace Core 
{

    /**
     * @brief Construct a new ArgsParser::ArgsParser object
     * 
     */
    ArgsParser::ArgsParser()
    {
        add_option(m_show_help, "Display help message and exit", "help", 0, OptionHideMode::Markdown);
        add_option(m_show_version, "Print version", "version", 0, OptionHideMode::Markdown);
        add_option(m_perform_autocomplete, "Perform autocompletion", "complete", 0, OptionHideMode::CommandLineAndMarkdown);
    }

    /**
     * @param argc 
     * @param argv 
     * @param failure_behavior 
     * @return true 
     * @return false 
     */
    bool ArgsParser::parse(int argc, char* const* argv, FailureBehavior failure_behavior)
    {
        auto fail = [this, argv, failure_behavior] {
            if (failure_behavior == FailureBehavior::PrintUsage || failure_behavior == FailureBehavior::PrintUsageAndExit)
                print_usage(stderr, argv[0]);
            if (failure_behavior == FailureBehavior::Exit || failure_behavior == FailureBehavior::PrintUsageAndExit)
                exit(1);
        };

        Vector<option> long_options;
        StringBuilder short_options_builder;

        if (m_stop_on_first_non_option)
            short_options_builder.append('+');

        int index_of_found_long_option = -1;

        optind = 0;

        for (size_t i = 0; i < m_options.size(); i++) {
            auto& opt = m_options[i];
            if (opt.long_name) {
                option long_opt {
                    opt.long_name,
                    opt.requires_argument ? required_argument : no_argument,
                    &index_of_found_long_option,
                    static_cast<int>(i)
                };
                long_options.append(long_opt);
            }
            if (opt.short_name) {
                short_options_builder.append(opt.short_name);
                if (opt.requires_argument)
                    short_options_builder.append(':');
            }
        }
        long_options.append({ 0, 0, 0, 0 });

        String short_options = short_options_builder.build();

        while (true) {
            int c = getopt_long(argc, argv, short_options.characters(), long_options.data(), nullptr);
            if (c == -1) {
                break;
            } else if (c == '?') {
                fail();
                return false;
            }

            Option* found_option = nullptr;
            if (c == 0) {
                VERIFY(index_of_found_long_option >= 0);
                found_option = &m_options[index_of_found_long_option];
                index_of_found_long_option = -1;
            } else {
                auto it = m_options.find_if([c](auto& opt) { return c == opt.short_name; });
                VERIFY(!it.is_end());
                found_option = &*it;
            }
            VERIFY(found_option);

            char const* arg = found_option->requires_argument ? optarg : nullptr;
            if (!found_option->accept_value(arg)) {
                warnln("\033[31mInvalid value for option \033[1m{}\033[22m\033[0m", found_option->name_for_display());
                fail();
                return false;
            }
        }

        if (m_show_version) {
            print_version(stdout);
            if (failure_behavior == FailureBehavior::Exit || failure_behavior == FailureBehavior::PrintUsageAndExit)
                exit(0);
            return false;
        }

        if (m_show_help) {
            print_usage(stdout, argv[0]);
            if (failure_behavior == FailureBehavior::Exit || failure_behavior == FailureBehavior::PrintUsageAndExit)
                exit(0);
            return false;
        }

        if (m_perform_autocomplete) {
            autocomplete(stdout, argv[0], Span<char const* const> { argv + optind, static_cast<size_t>(argc - optind) });
            if (failure_behavior == FailureBehavior::Exit || failure_behavior == FailureBehavior::PrintUsageAndExit)
                exit(0);
            return false;
        }

        int values_left = argc - optind;
        Vector<int, 16> num_values_for_arg;
        num_values_for_arg.resize(m_positional_args.size(), true);
        int total_values_required = 0;

        for (size_t i = 0; i < m_positional_args.size(); i++) {
            auto& arg = m_positional_args[i];
            num_values_for_arg[i] = arg.min_values;
            total_values_required += arg.min_values;
        }

        if (total_values_required > values_left) {
            fail();
            return false;
        }
        int extra_values_to_distribute = values_left - total_values_required;

        for (size_t i = 0; i < m_positional_args.size(); i++) {
            auto& arg = m_positional_args[i];
            int extra_values_to_this_arg = min(arg.max_values - arg.min_values, extra_values_to_distribute);
            num_values_for_arg[i] += extra_values_to_this_arg;
            extra_values_to_distribute -= extra_values_to_this_arg;
            if (extra_values_to_distribute == 0)
                break;
        }

        if (extra_values_to_distribute > 0) {
            fail();
            return false;
        }

        for (size_t i = 0; i < m_positional_args.size(); i++) {
            auto& arg = m_positional_args[i];
            for (int j = 0; j < num_values_for_arg[i]; j++) {
                char const* value = argv[optind++];
                if (!arg.accept_value(value)) {
                    warnln("Invalid value for argument {}", arg.name);
                    fail();
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * @param file 
     * @param argv0 
     */
    void ArgsParser::print_usage(FILE* file, char const* argv0)
    {
        char const* env_preference = getenv("ARGSPARSER_EMIT_MARKDOWN");

        if (env_preference != nullptr && env_preference[0] == '1' && env_preference[1] == 0) {
            print_usage_markdown(file, argv0);
        } else {
            print_usage_terminal(file, argv0);
        }
    }

    /**
     * @param file 
     * @param argv0 
     */
    void ArgsParser::print_usage_terminal(FILE* file, char const* argv0)
    {
        out(file, "Usage:\n\t\033[1m{}\033[0m", argv0);

        for (auto& opt : m_options) {
            if (opt.hide_mode != OptionHideMode::None)
                continue;
            if (opt.requires_argument)
                out(file, " [{} {}]", opt.name_for_display(), opt.value_name);
            else
                out(file, " [{}]", opt.name_for_display());
        }

        for (auto& arg : m_positional_args) {
            bool required = arg.min_values > 0;
            bool repeated = arg.max_values > 1;

            if (required && repeated)
                out(file, " <{}...>", arg.name);
            else if (required && !repeated)
                out(file, " <{}>", arg.name);
            else if (!required && repeated)
                out(file, " [{}...]", arg.name);
            else if (!required && !repeated)
                out(file, " [{}]", arg.name);
        }

        outln(file);

        if (m_general_help != nullptr && m_general_help[0] != '\0') {
            outln(file, "\nDescription:");
            outln(file, "{}", m_general_help);
        }

        if (!m_options.is_empty())
            outln(file, "\nOptions:");

        for (auto& opt : m_options) {
            if (opt.hide_mode == OptionHideMode::CommandLineAndMarkdown)
                continue;

            auto print_argument = [&]() {
                if (opt.value_name) {
                    if (opt.requires_argument)
                        out(file, " {}", opt.value_name);
                    else
                        out(file, " [{}]", opt.value_name);
                }
            };
            out(file, "\t");
            if (opt.short_name) {
                out(file, "\033[1m-{}\033[0m", opt.short_name);
                print_argument();
            }
            if (opt.short_name && opt.long_name)
                out(file, ", ");
            if (opt.long_name) {
                out(file, "\033[1m--{}\033[0m", opt.long_name);
                print_argument();
            }

            if (opt.help_string)
                out(file, "\t{}", opt.help_string);
            outln(file);
        }

        if (!m_positional_args.is_empty())
            outln(file, "\nArguments:");

        for (auto& arg : m_positional_args) {
            out(file, "\t\033[1m{}\033[0m", arg.name);
            if (arg.help_string)
                out(file, "\t{}", arg.help_string);
            outln(file);
        }
    }

    /**
     * @param file 
     * @param argv0 
     */
    void ArgsParser::print_usage_markdown(FILE* file, char const* argv0)
    {
        outln(file, "## Name\n\n{}", argv0);

        out(file, "\n## Synopsis\n\n```sh\n$ {}", argv0);
        for (auto& opt : m_options) {
            if (opt.hide_mode != OptionHideMode::None)
                continue;
            if (opt.requires_argument)
                out(file, " [{} {}]", opt.name_for_display(), opt.value_name);
            else
                out(file, " [{}]", opt.name_for_display());
        }
        for (auto& arg : m_positional_args) {
            bool required = arg.min_values > 0;
            bool repeated = arg.max_values > 1;

            if (required && repeated)
                out(file, " <{}...>", arg.name);
            else if (required && !repeated)
                out(file, " <{}>", arg.name);
            else if (!required && repeated)
                out(file, " [{}...]", arg.name);
            else if (!required && !repeated)
                out(file, " [{}]", arg.name);
        }
        outln(file, "\n```");

        if (m_general_help != nullptr && m_general_help[0] != '\0') {
            outln(file, "\n## Description\n\n{}", m_general_help);
        }

        auto should_display_option = [](Option& opt) {
            return !(opt.hide_mode == OptionHideMode::Markdown || opt.hide_mode == OptionHideMode::CommandLineAndMarkdown);
        };

        size_t options_to_display = 0;
        for (auto& opt : m_options) {
            if (!should_display_option(opt))
                continue;
            options_to_display++;
        }

        if (options_to_display > 0)
            outln(file, "\n## Options:\n");
        for (auto& opt : m_options) {
            if (!should_display_option(opt))
                continue;

            auto print_argument = [&]() {
                if (opt.value_name != nullptr) {
                    if (opt.requires_argument)
                        out(file, " {}", opt.value_name);
                    else
                        out(file, " [{}]", opt.value_name);
                }
            };
            out(file, "* ");
            if (opt.short_name != '\0') {
                out(file, "`-{}", opt.short_name);
                print_argument();
                out(file, "`");
            }
            if (opt.short_name != '\0' && opt.long_name != nullptr)
                out(file, ", ");
            if (opt.long_name != nullptr) {
                out(file, "`--{}", opt.long_name);
                print_argument();
                out(file, "`");
            }

            if (opt.help_string != nullptr)
                out(file, ": {}", opt.help_string);
            outln(file);
        }

        if (!m_positional_args.is_empty())
            outln(file, "\n## Arguments:\n");

        for (auto& arg : m_positional_args) {
            out(file, "* `{}`", arg.name);
            if (arg.help_string != nullptr)
                out(file, ": {}", arg.help_string);
            outln(file);
        }
    }

    /**
     * @param file 
     */
    void ArgsParser::print_version(FILE* file)
    {
        outln(file, Core::Version::PRANAOS_VERSION);
    }

    /**
     * @param option 
     */
    void ArgsParser::add_option(Option&& option)
    {
        m_options.append(move(option));
    }

    /**
     * @param long_name 
     * @param short_name 
     * @param hide_mode 
     */
    void ArgsParser::add_ignored(char const* long_name, char short_name, OptionHideMode hide_mode)
    {
        Option option {
            false,
            "Ignored",
            long_name,
            short_name,
            nullptr,
            [](char const*) {
                return true;
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(bool& value, char const* help_string, char const* long_name, char short_name, OptionHideMode hide_mode)
    {
        Option option {
            false,
            help_string,
            long_name,
            short_name,
            nullptr,
            [&value](char const* s) {
                VERIFY(s == nullptr);
                value = true;
                return true;
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(char const*& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&value](char const* s) {
                value = s;
                return true;
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(String& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&value](char const* s) {
                value = s;
                return true;
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(StringView& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&value](char const* s) {
                value = s;
                return true;
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(int& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&value](char const* s) {
                auto opt = StringView(s).to_int();
                value = opt.value_or(0);
                return opt.has_value();
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(unsigned& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&value](char const* s) {
                auto opt = StringView(s).to_uint();
                value = opt.value_or(0);
                return opt.has_value();
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(double& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&value](char const* s) {
                auto opt = convert_to_double(s);
                value = opt.value_or(0.0);
                return opt.has_value();
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(Optional<double>& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&value](char const* s) {
                value = convert_to_double(s);
                return value.has_value();
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param value 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param hide_mode 
     */
    void ArgsParser::add_option(Optional<size_t>& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&value](char const* s) {
                value = Mods::StringUtils::convert_to_uint<size_t>(s);
                return value.has_value();
            },
            hide_mode,
        };
        add_option(move(option));
    }

    /**
     * @param values 
     * @param help_string 
     * @param long_name 
     * @param short_name 
     * @param value_name 
     * @param separator 
     * @param hide_mode 
     */
    void ArgsParser::add_option(Vector<size_t>& values, char const* help_string, char const* long_name, char short_name, char const* value_name, char separator, OptionHideMode hide_mode)
    {
        Option option {
            true,
            help_string,
            long_name,
            short_name,
            value_name,
            [&values, separator](char const* s) {
                bool parsed_all_values = true;

                StringView { s }.for_each_split_view(separator, false, [&](auto value) {
                    if (auto maybe_value = Mods::StringUtils::convert_to_uint<size_t>(value); maybe_value.has_value())
                        values.append(*maybe_value);
                    else
                        parsed_all_values = false;
                });

                return parsed_all_values;
            },
            hide_mode
        };

        add_option(move(option));
    }

    /**
     * @param arg 
     */
    void ArgsParser::add_positional_argument(Arg&& arg)
    {
        m_positional_args.append(move(arg));
    }

    /**
     * @param value 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(char const*& value, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            1,
            [&value](char const* s) {
                value = s;
                return true;
            }
        };
        add_positional_argument(move(arg));
    }

    /**
     * @param value 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(String& value, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            1,
            [&value](char const* s) {
                value = s;
                return true;
            }
        };
        add_positional_argument(move(arg));
    }

    /**
     * @param value 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(StringView& value, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            1,
            [&value](char const* s) {
                value = s;
                return true;
            }
        };
        add_positional_argument(move(arg));
    }

    /**
     * @param value 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(int& value, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            1,
            [&value](char const* s) {
                auto opt = StringView(s).to_int();
                value = opt.value_or(0);
                return opt.has_value();
            }
        };
        add_positional_argument(move(arg));
    }

    /**
     * @param value 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(unsigned& value, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            1,
            [&value](char const* s) {
                auto opt = StringView(s).to_uint();
                value = opt.value_or(0);
                return opt.has_value();
            }
        };
        add_positional_argument(move(arg));
    }

    /**
     * @param value 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(double& value, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            1,
            [&value](char const* s) {
                auto opt = convert_to_double(s);
                value = opt.value_or(0.0);
                return opt.has_value();
            }
        };
        add_positional_argument(move(arg));
    }   

    /**
     * @param values 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(Vector<char const*>& values, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            INT_MAX,
            [&values](char const* s) {
                values.append(s);
                return true;
            }
        };
        add_positional_argument(move(arg));
    }

    /**
     * @param values 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(Vector<String>& values, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            INT_MAX,
            [&values](char const* s) {
                values.append(s);
                return true;
            }
        };
        add_positional_argument(move(arg));
    }
    
    /**
     * @param values 
     * @param help_string 
     * @param name 
     * @param required 
     */
    void ArgsParser::add_positional_argument(Vector<StringView>& values, char const* help_string, char const* name, Required required)
    {
        Arg arg {
            help_string,
            name,
            required == Required::Yes ? 1 : 0,
            INT_MAX,
            [&values](char const* s) {
                values.append(s);
                return true;
            }
        };
        add_positional_argument(move(arg));
    }

    /**
     * @param file 
     * @param program_name 
     * @param remaining_arguments 
     */
    void ArgsParser::autocomplete(FILE* file, StringView program_name, Span<char const* const> remaining_arguments)
    {
        auto first = true;
        auto seen_all_options = false;
        auto skip_next = false;

        StringView argument_to_complete;
        StringView option_to_complete;
        auto completing_option = false;

        for (auto& arg : remaining_arguments) {
            StringView argument { arg };

            completing_option = false;
            if (skip_next) {
                argument_to_complete = argument;
                skip_next = false;
                continue;
            }

            if (first && program_name == argument) {
                first = false;
                continue;
            }

            if (seen_all_options) {
                argument_to_complete = argument;
                continue;
            }

            if (argument.starts_with("--")) {
                option_to_complete = argument;
                completing_option = true;

                if (argument == "--") {
                    seen_all_options = true;
                    continue;
                }

                auto option_pattern = argument.substring_view(2);
                auto it = m_options.find_if([&](auto& option) { return option.hide_mode != OptionHideMode::None && StringView(option.long_name) == option_pattern; });
                if (it.is_end())
                    continue;

                if (it->requires_argument)
                    skip_next = true;
                continue;
            }

            if (argument.starts_with("-")) {
                option_to_complete = argument;
                completing_option = true;

                if (argument == "-") {
                    option_to_complete = argument;
                    continue;
                }

                auto option_pattern = argument[argument.length() - 1];
                auto it = m_options.find_if([&](auto& option) { return option.hide_mode != OptionHideMode::None && option.short_name == option_pattern; });

                if (it.is_end())
                    continue;

                if (it->requires_argument)
                    skip_next = true;
                continue;
            }
        }

        if (!completing_option)
            return;

        auto write_completion = [&](auto format, auto& option, auto has_invariant, auto... args) {
            JsonObject object;
            object.set("completion", String::formatted(format, args...));
            object.set("static_offset", 0);
            object.set("invariant_offset", has_invariant ? option_to_complete.length() : 0u);
            object.set("display_trivia", option.help_string);
            object.set("trailing_trivia", option.requires_argument ? " " : "");
            outln(file, "{}", object.to_string());
        };

        if (option_to_complete.starts_with("--")) {
            auto option_pattern = option_to_complete.substring_view(2);
            for (auto& option : m_options) {
                if (option.hide_mode != OptionHideMode::None)
                    continue;
                StringView option_string = option.long_name;
                if (option_string.starts_with(option_pattern)) {
                    write_completion("--{}", option, true, option_string);
                }
            }
        } else {
            for (auto& option : m_options) {
                if (option.hide_mode != OptionHideMode::None)
                    continue;
                if (option.short_name == 0)
                    continue;

                auto has_invariant = option_to_complete == "-";
                write_completion("{}{}", option, has_invariant, has_invariant ? "-" : "", option.short_name);
            }
        }
    }

} // namespace Core
