/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Utils {

template <typename T>
class Badge {
public:
    using Type = T;

private:
    constexpr Badge() = default;

    Badge(const Badge&) = default;

    Badge(const Badge&) = delete;
    Badge& operator=(const Badge&) = delete;

};

}

using Utils::Badge;