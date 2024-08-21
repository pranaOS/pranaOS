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

/// @brief args
#define no_argument 0
#define required_argument 1
#define optional_argument 2


/// @brief  option
struct option {
    const char* name;
    int has_arg;
    int* flag;
    int val;
};


/**
 * @param argc 
 * @param argv 
 * @param short_options 
 * @param long_options 
 * @param out_long_option_index 
 * @return int 
 */
int getopt_long(int argc, char** argv, const char* short_options, const struct option* long_options, int* out_long_option_index);

__END_DECLS
