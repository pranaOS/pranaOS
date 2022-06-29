#pragma once

#include <glob.h>
#include <cstring>
#include <filesystem>

class Glob {
    struct Fragment {
        std::filesystem::path path;
        bool recursive;
    };

public:
    explict Glob(const std::string& pattern) 
    {
        split_into_fragments(pattern);
        search();
    }

    std::vector<std::filesystem::path>&& pat { return std::move(m_result); }
}