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

    namespace Dir {
        std::filesystem::path getSystemDiskPath() noexcept;
        std::filesystem::path getUserDiskPath() noexcept;
        std::filesystem::path getMfgConfPath() noexcept;
        std::filesystem::path getDatabasesPath() noexcept;
        std::filesystem::path getLogsPath() noexcept;
        std::filesystem::path getCrashDumpsPath() noexcept;
        std::filesystem::path getUserMediaPath() noexcept;
        std::filesystem::path getTemporaryPath() noexcept;
        std::filesystem::path getBootJSONPath() noexcept;
        std::filesystem::path getAssetsDirPath() noexcept;
        std::filesystem::path getSystemDataDirPath() noexcept;
        std::filesystem::path getSystemVarDirPath() noexcept;
    } 

    namespace File {
        constexpr inline auto boot_json = "bootcfg.json";
        constexpr inline auto os_bin = "os.bin";
        constexpr inline auto version_json = "version.json";
    }

    namespace Buffer {
        constexpr inline auto tar_buf = 1024;
    }
}