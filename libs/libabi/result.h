/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

enum JResult
{
#define RESULT_ENUM_ENTRY(__name, __description) __name,
    RESULT_ENUM(RESULT_ENUM_ENTRY)
#undef RESULT_ENUM_ENTRY
    __RESULT_COUNT
};

#define result_is_error(__result) ((__result) != SUCCESS && (__result) != TIMEOUT)

static inline const char *result_to_string(JResult error)
{
#define RESULT_ENUM_ENTRY_STRING(__entry, __description) #__entry,
    const char *RESULT_NAMES[] = {RESULT_ENUM(RESULT_ENUM_ENTRY_STRING)};
#undef RESULT_ENUM_ENTRY

    if (error < __RESULT_COUNT && error >= 0)
    {
        return RESULT_NAMES[error];
    }
    else
    {
        return "INVALID_RESULT_CODE";
    }
}
