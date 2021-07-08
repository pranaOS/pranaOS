/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Time.h>
#include <libutils/Prelude.h>

#define SYSTEM_INFO_FIELD_SIZE 65

struct SystemInfo
{
    char kernel_name[SYSTEM_INFO_FIELD_SIZE];    
    char kernel_release[SYSTEM_INFO_FIELD_SIZE]; 
    char kernel_build[SYSTEM_INFO_FIELD_SIZE];
    char system_name[SYSTEM_INFO_FIELD_SIZE]; 
    char machine[SYSTEM_INFO_FIELD_SIZE];     
};