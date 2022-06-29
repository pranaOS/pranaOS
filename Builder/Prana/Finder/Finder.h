#pragma once

#include "../Utils/Utils.h"
#include "Glob.h"
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>


class Finder {
    static constexpr auto PranaExtensionPattern = "*.prana";

public:
    static inline auto FindRootPranaFiles()
    {
        return Glob(PranaExtensionPattern).result();
    }
    
    static inline auto FindPranaFiles(const std::string& directory, const std::string& pattern = "")
    {
        auto path = std::filesystem::path(directory) / std::filesystem::path(pattern) / PranaExtensionPattern;
        return Glob(path).result();
    }

    static inline auto FindFiles(const std::string& directory, const std::string& pattern)
    {
        auto path = std::filesystem::path(directory) / std::filesystem::path(pattern);
        return Glob(path).result();
    }

    static inline auto GetExtension(const std::string& filename)
    {
        return std::filesystem::path(filename).extension();
    }

    static inline auto GetFolder(const std::string& path)
    {
        return std::filesystem::path(path).parent_path();
    }

    static inline void CreateDirectory(const std::string& directory)
    {
        auto dirs = Utils::Split(directory, "/");
        int begin = 0;
        if (!directory.empty() && directory[0] == '/') {
            begin++;
        }

        std::filesystem::path cur_path = dirs[begin];

        for (size_t at = begin + 1; at < dirs.size(); at++) {
            if (!std::filesystem::exists(cur_path)) {
                std::filesystem::create_directory(cur_path);
            }

            cur_path = cur_path / dirs[at];
        }

        if (!std::filesystem::exists(cur_path)) {
            std::filesystem::create_directory(cur_path);
        }
    }
};