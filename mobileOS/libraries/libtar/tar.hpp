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

    class entry {
        
      public:
        ~entry();

        /**
         * @brief size
         * 
         * @return std::size_t 
         */
        std::size_t size() const;

        /**
         * @brief name
         * 
         * @return std::filesystem::path 
         */
        std::filesystem::path name() const;

        /**
         * @brief isFile, isDirectory
         * 
         * @return true 
         * @return false 
         */
        bool isFile() const;
        bool isDirectory() const;
        
        /**
         * @brief read
         * 
         * @param data 
         * @param size 
         */
        void read(const std::byte *data, std::size_t size) const;

      private: 
        /**
         * @brief Construct a new entry object
         * 
         * @param path 
         */
        explicit entry(const std::filesystem::path &path);
        friend class iterator;
        mutable mtar_t handle{};
        mtar_header_t tar_header;
    };

    class iterator {
      public:
        using iterator_category = std::input_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = entry;
        using pointer           = entry *;
        using reference         = entry &;

        iterator() = default;

        /**
         * @brief Construct a new iterator object
         * 
         * @param path 
         */
        explicit iterator(const std::filesystem::path &path);

        /**
         * @brief operator[reference, pointer]
         * 
         * @return reference 
         */
        reference operator*() const;
        pointer operator->() const;

        iterator &operator++();
        iterator operator++(int);

        /**
         * @brief operator
         * 
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        friend bool operator==(const iterator &a, const iterator &b);
        friend bool operator!=(const iterator &a, const iterator &b);

      private:
        std::shared_ptr<entry> entry_;
    };

    /**
     * @brief begin
     * 
     * @param it 
     * @return iterator 
     */
    inline iterator begin(const iterator &it) noexcept {
        return it;
    }

    /**
     * @brief end
     * 
     * @return iterator 
     */
    inline iterator end(iterator) noexcept {
        return iterator{};
    }

    /**
     * @brief unpack
     * 
     * @param path 
     * @param where 
     */
    void unpack(const std::filesystem::path &path, const std::filesystem::path &where = {});
}
