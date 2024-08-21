/**
 * @file getopt.cpp
 * @author Krisna Pranav
 * @brief GETOPT
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mods/string_view.h>
#include <mods/vector.h>


int opterr = 1;
int optopt = 0;
int optind = 1;
int optreset = 0;
char* optarg = nullptr;

// POSIX says, "When an element of argv[] contains multiple option characters,
// it is unspecified how getopt() determines which options have already been
// processed". Well, this is how we do it.
static size_t s_index_into_multioption_argument = 0;

static inline void report_error(const char* format, ...)
{
    if (!opterr)
        return;

    fputs("\033[31m", stderr);

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fputs("\033[0m\n", stderr);
}

namespace 
{

    class OptionParser 
    {
    public:

        /**
         * @param argc 
         * @param argv 
         * @param short_options 
         * @param long_options 
         * @param out_long_option_index 
         */
        OptionParser(int argc, char** argv, const StringView& short_options, const option* long_options, int* out_long_option_index = nullptr);
        int getopt();

    private:

        /**
         * @param option 
         * @param needs_value 
         * @return true 
         * @return false 
         */
        bool lookup_short_option(char option, int& needs_value) const;

        /**
         * @return int 
         */
        int handle_short_option();

        /**
         * @param raw 
         * @return const option* 
         */
        const option* lookup_long_option(char* raw) const;

        /**
         * @return int 
         */
        int handle_long_option();

        void shift_argv();
        bool find_next_option();

        size_t m_argc { 0 };
        char** m_argv { nullptr };

        StringView m_short_options;

        const option* m_long_options { nullptr };
        int* m_out_long_option_index { nullptr };
        bool m_stop_on_first_non_option { false };

        size_t m_arg_index { 0 };
        size_t m_consumed_args { 0 };
    }; // class OptionParser

    /**
     * @param argc 
     * @param argv 
     * @param short_options 
     * @param long_options 
     * @param out_long_option_index 
     */
    OptionParser::OptionParser(int argc, char** argv, const StringView& short_options, const option* long_options, int* out_long_option_index)
        : m_argc(argc)
        , m_argv(argv)
        , m_short_options(short_options)
        , m_long_options(long_options)
        , m_out_long_option_index(out_long_option_index)
    {
        m_stop_on_first_non_option = short_options.starts_with('+');

        if (optreset || optind == 0) {
            optreset = 0;
            optind = 1;
            s_index_into_multioption_argument = 0;
        }

        optopt = 0;
        optarg = nullptr;
    }

    /**
     * @return int 
     */
    int OptionParser::getopt()
    {
        bool should_reorder_argv = !m_stop_on_first_non_option;
        int res = -1;

        bool found_an_option = find_next_option();
        StringView arg = m_argv[m_arg_index];

        if (!found_an_option) {
            res = -1;
            if (arg == "--")
                m_consumed_args = 1;
            else
                m_consumed_args = 0;
        } else {
            bool is_long_option = arg.starts_with("--");
            if (is_long_option)
                res = handle_long_option();
            else
                res = handle_short_option();

            if (res == '?')
                return '?';
        }

        if (should_reorder_argv)
            shift_argv();
        else
            ASSERT(optind == static_cast<int>(m_arg_index));
        optind += m_consumed_args;

        return res;
    }

    /**
     * @param option 
     * @param needs_value 
     * @return true 
     * @return false 
     */
    bool OptionParser::lookup_short_option(char option, int& needs_value) const
    {
        Vector<StringView> parts = m_short_options.split_view(option, true);

        ASSERT(parts.size() <= 2);
        if (parts.size() < 2) {
            return false;
        }

        if (parts[1].starts_with("::")) {
            needs_value = optional_argument;
        } else if (parts[1].starts_with(':')) {
            needs_value = required_argument;
        } else {
            needs_value = no_argument;
        }
        return true;
    }

    /**
     * @return int 
     */
    int OptionParser::handle_short_option()
    {
        StringView arg = m_argv[m_arg_index];
        ASSERT(arg.starts_with('-'));

        if (s_index_into_multioption_argument == 0) {
            s_index_into_multioption_argument = 1;
        }

        char option = arg[s_index_into_multioption_argument];
        s_index_into_multioption_argument++;

        int needs_value = no_argument;
        bool ok = lookup_short_option(option, needs_value);
        if (!ok) {
            optopt = option;
            report_error("Unrecognized option \033[1m-%c\033[22m", option);
            return '?';
        }

        if (s_index_into_multioption_argument < arg.length()) {
            if (needs_value == no_argument) {
                optarg = nullptr;
                m_consumed_args = 0;
            } else {
                optarg = m_argv[m_arg_index] + s_index_into_multioption_argument;
                s_index_into_multioption_argument = 0;
                m_consumed_args = 1;
            }
        } else {
            s_index_into_multioption_argument = 0;
            if (needs_value != required_argument) {
                optarg = nullptr;
                m_consumed_args = 1;
            } else if (m_arg_index + 1 < m_argc) {
                optarg = m_argv[m_arg_index + 1];
                m_consumed_args = 2;
            } else {
                report_error("Missing value for option \033[1m-%c\033[22m", option);
                return '?';
            }
        }

        return option;
    }

    /**
     * @param raw 
     * @return const option* 
     */
    const option* OptionParser::lookup_long_option(char* raw) const
    {
        StringView arg = raw;

        for (size_t index = 0; m_long_options[index].name; index++) {
            auto& option = m_long_options[index];
            StringView name = option.name;

            if (!arg.starts_with(name))
                continue;

            if (m_out_long_option_index)
                *m_out_long_option_index = index;

            if (arg.length() == name.length()) {
                optarg = nullptr;
                return &option;
            }
            ASSERT(arg.length() > name.length());
            if (arg[name.length()] == '=') {
                optarg = raw + name.length() + 1;
                return &option;
            }
        }

        return nullptr;
    }

    /**
     * @return int 
     */
    int OptionParser::handle_long_option()
    {
        ASSERT(StringView(m_argv[m_arg_index]).starts_with("--"));

        optopt = 0;

        auto* option = lookup_long_option(m_argv[m_arg_index] + 2);
        if (!option) {
            report_error("Unrecognized option \033[1m%s\033[22m", m_argv[m_arg_index]);
            return '?';
        }

        switch (option->has_arg) {
        case no_argument:
            if (optarg) {
                report_error("Option \033[1m--%s\033[22m doesn't accept an argument", option->name);
                return '?';
            }
            m_consumed_args = 1;
            break;
        case optional_argument:
            m_consumed_args = 1;
            break;
        case required_argument:
            if (optarg) {
                m_consumed_args = 1;
            } else if (m_arg_index + 1 < m_argc) {

                optarg = m_argv[m_arg_index + 1];
                m_consumed_args = 2;
            } else {
                report_error("Missing value for option \033[1m--%s\033[22m", option->name);
                return '?';
            }
            break;
        default:
            ASSERT_NOT_REACHED();
        }

        if (option->flag) {
            *option->flag = option->val;
            return 0;
        }
        return option->val;
    }

    void OptionParser::shift_argv()
    {
        ASSERT(optind <= static_cast<int>(m_arg_index));

        if (optind == static_cast<int>(m_arg_index) || m_consumed_args == 0) {
            return;
        }

        char* buffer[m_consumed_args];
        memcpy(buffer, &m_argv[m_arg_index], sizeof(char*) * m_consumed_args);
        memmove(&m_argv[optind + m_consumed_args], &m_argv[optind], sizeof(char*) * (m_arg_index - optind));
        memcpy(&m_argv[optind], buffer, sizeof(char*) * m_consumed_args);
    }

    /**
     * @return true 
     * @return false 
     */
    bool OptionParser::find_next_option()
    {
        for (m_arg_index = optind; m_arg_index < m_argc && m_argv[m_arg_index]; m_arg_index++) {
            StringView arg = m_argv[m_arg_index];
            if (!arg.starts_with('-')) {
                if (m_stop_on_first_non_option)
                    return false;
                continue;
            }

            if (arg == "-")
                continue;

            if (arg == "--")
                return false;

            return true;
        }

        return false;
    }

} // namespace

/**
 * @param argc 
 * @param argv 
 * @param short_options 
 * @return int 
 */
int getopt(int argc, char** argv, const char* short_options)
{
    option dummy { nullptr, 0, nullptr, 0 };
    OptionParser parser { argc, argv, short_options, &dummy };
    return parser.getopt();
}

/**
 * @param argc 
 * @param argv 
 * @param short_options 
 * @param long_options 
 * @param out_long_option_index 
 * @return int 
 */
int getopt_long(int argc, char** argv, const char* short_options, const struct option* long_options, int* out_long_option_index)
{
    OptionParser parser { argc, argv, short_options, long_options, out_long_option_index };
    return parser.getopt();
}
