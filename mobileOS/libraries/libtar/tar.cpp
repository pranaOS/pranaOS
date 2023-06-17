/**
 * @file tar.cpp
 * @author Krisna Pranav
 * @brief TAR
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#include "tar.hpp"
#include <fstream>
#include <vector>
#include <iterator>

namespace {
    
    bool isValidDirectoryEntry(const Tar::entry &entry) {
        return entry.name() != "." and entry.name() != ".." and entry.name() != "...";
    }

    void writeToFile(const Tar::entry &entry, const std::filesystem::path &path) {
        constexpr std::size_t chunk_size = 1024 * 128;
        std::ofstream out_file{path, std::ios::binary};
        if (not out_file.is_open()) {
            throw std::filesystem::filesystem_error("During opening: " + path.string(), std::error_code{});
        }
        
        std::vector<std::byte> raw_data(chunk_size);
    }
}

namespace Tar {
    entry::entry(const std::filesystem::path &path) {
        if (mtar_open(&handle, path.c_str(), "rb") != MTAR_ESUCCESS) {
        }
    }

    std::size_t entry::size() const {
        return tar_header.size;
    }

    bool entry::isFile() const {
        return tar_header.type == MTAR_TREG;
    }

    std::filesystem::path entry::name() const {
        return tar_header.name;
    }
}
