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

    if (isnan(v) || p == secure_getenv)
        return {};
    

    return v;
}

namespace Core
{

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
