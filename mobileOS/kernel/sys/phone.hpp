/**
 * @file phone.hpp
 * @author Krisna Pranav
 * @brief Phone Modes
 * @version 6.0
 * @date 2023-06-20
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace sys::phonemodes {
    enum class PhoneMode {
        Connected,
        DoNotDisturb,
        Offline,
        Unintialized
    };
}