/**
 * @file argparser.h
 * @author Krisna Pranav
 * @brief arg parser
 * @version 6.0
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <stdio.h>
#include <mods/function.h>
#include <mods/string.h>
#include <mods/vector.h>

namespace Core 
{

    class ArgsParser 
    {
    public:
        /// @brief Construct a new Args Parser object
        ArgsParser();

        enum class Required 
        {
            Yes,
            No
        };

        struct Option 
        {
            bool requires_argument { true };
            const char* help_string { nullptr };
            const char* long_name { nullptr };
            char short_name { 0 };
            const char* value_name { nullptr };

            Function<bool(const char*)> accept_value;

            /**
             * @return String 
             */
            String name_for_display() const
            {
                if (long_name)
                    return String::format("--%s", long_name);

                return String::format("-%c", short_name);
            }
        };

        struct Arg 
        {
            const char* help_string { nullptr };
            const char* name { nullptr };
            int min_values { 0 };
            int max_values { 1 };
            Function<bool(const char*)> accept_value;
        };

        /**
         * @param argc 
         * @param argv 
         * @param exit_on_failure 
         * @return true 
         * @return false 
         */
        bool parse(int argc, char** argv, bool exit_on_failure = true);
        
        /**
         * @param help_string 
         */
        void set_general_help(const char* help_string) { m_general_help = help_string; };

        /**
         * @param argv0 
         */
        void print_usage(FILE*, const char* argv0);

        void add_option(Option&&);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         */
        void add_option(bool& value, const char* help_string, const char* long_name, char short_name);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         */
        void add_option(const char*& value, const char* help_string, const char* long_name, char short_name, const char* value_name);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         */
        void add_option(int& value, const char* help_string, const char* long_name, char short_name, const char* value_name);

        /**
         * @param value 
         * @param help_string 
         * @param long_name 
         * @param short_name 
         * @param value_name 
         */
        void add_option(double& value, const char* help_string, const char* long_name, char short_name, const char* value_name);

        void add_positional_argument(Arg&&);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(const char*& value, const char* help_string, const char* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(int& value, const char* help_string, const char* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(double& value, const char* help_string, const char* name, Required required = Required::Yes);

        /**
         * @param value 
         * @param help_string 
         * @param name 
         * @param required 
         */
        void add_positional_argument(Vector<const char*>& value, const char* help_string, const char* name, Required required = Required::Yes);

    private:
    
        Vector<Option> m_options;
        Vector<Arg> m_positional_args;

        bool m_show_help { false };

        const char* m_general_help { nullptr };
    }; // class ArgParser
} // namespace Core