/**
 * @file getopt.h
 * @author Krisna Pranav
 * @brief getopt
 * @version 1.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

struct option {
    const char* name;
    int has_arg;
    int* flag;
};

/**
 * @brief Get the opt long objc
 * 
 * @param argc 
 * @param argv 
 * @param short_options 
 * @param long_options 
 * @return int 
 */
int getopt_long(int argc, char** argv, const char* short_options, const struct option* long_options);

__END_DECLS