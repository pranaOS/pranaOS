/**
 * @file getopt.h
 * @author Krisna Pranav
 * @brief getopt
 * @version 6.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

#define no_argument 0
#define required_argument 1
#define optional_argument 2

struct option {
    char const* name;
    int has_arg;
    int* flag;
    int val;
}; // struct option

extern int opterr;
extern int optopt;
extern int optind;
extern int optreset;
extern char* optarg;

/**
 * @brief Get the opt long object
 * 
 * @param argc 
 * @param argv 
 * @param short_options 
 * @param long_options 
 * @param out_long_option_index 
 * @return int 
 */
int getopt_long(int argc, char* const* argv, char const* short_options, const struct option* long_options, int* out_long_option_index);

__END_DECLS
