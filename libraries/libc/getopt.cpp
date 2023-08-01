/**
 * @file getopt.cpp
 * @author Krisna Pranav
 * @brief GETOPT
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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

static size_t s_index_into_multioption_argument = 0;

/**
 * @param format 
 * @param ... 
 */
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
        bool lookup_short_options(char option, int& needs_value) const;

        int handle_short_option();

        const option* lookup_long_option(char* raw) const;

        int handle_long_option();

        void shift_argv();

        bool find_next_option();

        size_t m_argc { 0 };
    }; // class OptionParser

    /**
     * @return int 
     */
    int OptionParser::getopt()
    {
    }
} // namespace