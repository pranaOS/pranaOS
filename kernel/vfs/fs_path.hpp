/**
 * @file fs_path.hpp
 * @author Krisna Pranav
 * @brief FileSystem Path
 * @version 1.0
 * @date 2023-06-20
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <filesystem>

namespace FSPath {
    namespace dir {
        std::filesystem::path getSystemDiskPath() noexcept;
        // std::filesystem::path 
    }
}