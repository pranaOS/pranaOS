/**
 * @file stdbool.h
 * @author Krisna Pranav
 * @brief stdbool
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#ifndef __cplusplus
#   include <sys/cdefs.h>

__BEGIN_DECLS

#   define bool _Bool
#   define true 1
#   define false 0

__END_DECLS 


#endif 