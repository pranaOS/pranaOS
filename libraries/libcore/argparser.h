/**
 * @file argparser.h
 * @author Krisna Pranav
 * @brief arg parser
 * @version 6.0
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <mods/string.h>
#include <mods/vector.h>
#include <libmain/main.h>
#include <stdio.h>

namespace Core 
{

    class ArgsParser 
    {
    public:
        /**
         * @brief Construct a new ArgsParser object
         * 
         */
        ArgsParser();

        enum class Required 
        {
            Yes,
            No
        }; // enum class Required 

        enum class FailureBehavior 
        {
            PrintUsageAndExit,
            PrintUsage,
            Exit,
            Ignore,
        }; // enum class FailureBehavior 

        enum class OptionHideMode 
        {
            None,
            Markdown,
            CommandLineAndMarkdown,
        }; // enum class OptionHideMode 

        struct Option 
        {
            bool requires_argument { true };
            char const* help_string { nullptr };
            char const* long_name { nullptr };
            char short_name { 0 };
            char const* value_name { nullptr };
            Function<bool(char const*)> accept_value;
            OptionHideMode hide_mode { OptionHideMode::None };

            /**
             * @return String 
             */
            String name_for_display() const
            {
                if (long_name)
                    return String::formatted("--{}", long_name);
                return String::formatted("-{:c}", short_name);
            }
        }; // struct Option 

        struct Arg 
        {
            char const* help_string { nullptr };
            char const* name { nullptr };
            int min_values { 0 };
            int max_values { 1 };
            Function<bool(char const*)> accept_value;
        }; // struct Arg 

        /**
         * @param argc 
         * @param argv 
         * @param failure_behavior 
         * @return true 
         * @return false 
         */
        bool parse(int argc, char* const* argv, FailureBehavior failure_behavior = FailureBehavior::PrintUsageAndExit);

        /**
         * @param arguments 
         * @param failure_behavior 
         * @return true 
         * @return false 
         */
        bool parse(Main::Arguments const& arguments, FailureBehavior failure_behavior = FailureBehavior::PrintUsageAndExit)
        {
            return parse(arguments.argc, arguments.argv, failure_behavior);
        }

        /**
         * @brief Set the general help object
         * 
         * @param help_string 
         */
        void set_general_help(char const* help_string) 
        { 
            m_general_help = help_string; 
        };

        /**
         * @brief Set the stoponfirstnonoption object
         * 
         * @param stop_on_first_non_option 
         */
        void set_stop_on_first_non_option(bool stop_on_first_non_option) 
        { 
            m_stop_on_first_non_option = stop_on_first_non_option; 
        }

        /**
         * @param argv0 
         */
        void print_usage(FILE*, char const* argv0);

        /**
         * @param argv0 
         */
        void print_usage_terminal(FILE*, char const* argv0);

        /**
         * @param argv0 
         */
        void print_usage_markdown(FILE*, char const* argv0);

        void print_version(FILE*);

        void add_option(Option&&);

        /**
         * @param long_name 
         * @param short_name 
         * @param hide_mode 
         */
        void add_ignored(char const* long_name, char short_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param hide_mode 
         */
        void add_option(bool& value, char const* help_string, char const* long_name, char short_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param hide_mode 
         */
        void add_option(char const*& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param hide_mode 
         */
        void add_option(String& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param hide_mode 
         */
        void add_option(StringView& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param hide_mode 
         */
        void add_option(int& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param hide_mode 
         */
        void add_option(unsigned& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param hide_mode 
         */
        void add_option(double& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param hide_mode 
         */
        void add_option(Optional<double>& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param hide_mode 
         */
        void add_option(Optional<size_t>& value, char const* help_string, char const* long_name, char short_name, char const* value_name, OptionHideMode hide_mode = OptionHideMode::None);

        /**
         * @param values 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         * @param separator 
         * @param hide_mode 
         */
        void add_option(Vector<size_t>& values, char const* help_string, char const* long_name, char short_name, char const* value_name, char separator = ',', OptionHideMode hide_mode = OptionHideMode::None);

        void add_positional_argument(Arg&&);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(char const*& value, char const* help_string, char const* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(String& value, char const* help_string, char const* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(StringView& value, char const* help_string, char const* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(int& value, char const* help_string, char const* name, Required required = Required::Yes);

        /** 
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(unsigned& value, char const* help_string, char const* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(double& value, char const* help_string, char const* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(Vector<char const*>& value, char const* help_string, char const* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(Vector<String>& value, char const* help_string, char const* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(Vector<StringView>& value, char const* help_string, char const* name, Required required = Required::Yes);

    private:
        /**
         * @param program_name 
         * @param remaining_arguments 
         */
        void autocomplete(FILE*, StringView program_name, Span<char const* const> remaining_arguments);

        Vector<Option> m_options;
        Vector<Arg> m_positional_args;

        bool m_show_help { false };
        bool m_show_version { false };
        bool m_perform_autocomplete { false };
        char const* m_general_help { nullptr };
        bool m_stop_on_first_non_option { false };
    }; // class ArgsParser

} // namespace Core
