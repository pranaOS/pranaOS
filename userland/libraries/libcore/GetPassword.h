/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/OSError.h>
#include <base/Result.h>
#include <base/String.h>

namespace Core {

Result<String, OSError> get_password(const StringView& prompt = "Password: ");

}
