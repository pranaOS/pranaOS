/*
 * Copyright (c) 2021, Krisna Pranav
 * Copyright (c) 2020, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

namespace Utils {

template<typename T>
class Badge {
public:
    using Type = T;

private:
    friend T;
    constexpr Badge() = default;

    Badge(const Badge&) = delete;
    Badge& operator=(const Badge&) = delete;

    Badge(Badge&&) = delete;
    Badge& operator=(Badge&&) = delete;
};

}

using Utils::Badge;