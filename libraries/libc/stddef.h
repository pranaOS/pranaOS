/**
 * @file stddef.h
 * @author Krisna Pranav
 * @brief stddef
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#ifndef KERNEL

#   ifdef __cplusplus
#       define NULL nullptr
#   else
#       define NULL ((void*)0)
#   endif 

typedef __SIZE_TYPE__ size_t;

#endif