/**
 * @file resolv.h
 * @author Krisna Pranav
 * @brief resolv
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/types.h>

__BEGIN_DECLS

/**
 * @param dname 
 * @param class_ 
 * @param type 
 * @param answer 
 * @param anslen 
 * @return int 
 */
int res_query(char const* dname, int class_, int type, unsigned char* answer, int anslen);

__END_DECLS
