/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#define TRY(expression)                               \
    ({                                                \
        auto _temporary_result = (expression);        \
        if (_temporary_result.is_error())             \
            return _temporary_result.release_error(); \
        _temporary_result.release_value();            \
    })