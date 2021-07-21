/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "AK/StdLibExtras.h"


#define BASE_ENUM_BITWISE_OPERATORS(Enum) \
        _BASE_ENUM_BITWISE_OPERATOR_INTERNAL(Enum, )

#define AK_ENUM_BITWISE_FRIEND_OPERATORS(Enum) \
    _AK_ENUM_BITWISE_OPERATORS_INTERNAL(Enum, friend)