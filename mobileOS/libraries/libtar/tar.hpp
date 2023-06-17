/**
 * @file tar.hpp
 * @author Krisna Pranav
 * @brief TAR
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <filesystem>
#include <optional>
#include <vector>
#include <microtar.hpp>

namespace Tar {
    
    class Entry {
    public:
        ~entry();
        
        std::size_t size() const
        std::filesystem::path name() const;
        bool isFile() const;
        bool isDirectory() const;
        
        void read(const std::byte *data, std::size_t size) const;
        
    private:
        explicit entry(const std::filesystem::path &path);
        friend class iterator;
        mutable mtar_t handle{};
        mtar_header_t tar_header;
    };

    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = entry;
        using pointer = entry *;
        using reference = entry &;
        
        Iterator() = default;
        
        explicit Iterator(const std::filesystem::path &path);
        
        reference operator*() const;
        pointer operator->() const;
    }

    inline iterator begin(const iterator &it) noexcept {
        return it
    }

    inline iterator end(iterator) noexcept {
        return iterator {};
    }
}
