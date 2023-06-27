/**
 * @file config.h
 * @author Krisna Pranav
 * @brief Config
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define SQLITE_OS_OTHER     1   
#define SQLITE_TEMP_STORE   3   
#define SQLITE_THREADSAFE   0   
#define SQLITE_MEMDEBUG     0   
#define SQLITE_OMIT_AUTOINIT 1  
#define SQLITE_DEFAULT_MEMSTATUS 0

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Woverflow"