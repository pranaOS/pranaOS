/**
 * @file tar.cpp
 * @author Krisna Pranav
 * @brief TAR
 * @version 0.1
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#include "tar.hpp"
#include <fstream>
#include <iterator>
#include <vector>

namespace
{
/**
 * @brief isValidDirEntry
 *
 * @param entry
 * @return true
 * @return false
 */
bool isValidDirectoryEntry(const Tar::entry& entry)
{
    return entry.name() != "." and entry.name() != ".." and entry.name() != "...";
}

/**
 * @brief writeToFile
 *
 * @param entry
 * @param path
 */
void writeToFile(const Tar::entry& entry, const std::filesystem::path& path)
{
    constexpr std::size_t chunk_size = 1024 * 128;
    std::ofstream out_file{path, std::ios::binary};
    if(not out_file.is_open())
    {
        throw std::filesystem::filesystem_error("During opening: " + path.string(), std::error_code{});
    }

    std::vector<std::byte> raw_data(chunk_size);
    auto bytes_left = entry.size();
    while(bytes_left > 0)
    {
        const std::size_t to_read = bytes_left > chunk_size ? chunk_size : bytes_left;
        entry.read(raw_data.data(), to_read);

        out_file.write(reinterpret_cast<const char*>(raw_data.data()), to_read);
        bytes_left -= to_read;
    }
}
} // namespace

namespace Tar
{
/**
 * @brief Construct a new entry::entry object
 *
 * @param path
 */
entry::entry(const std::filesystem::path& path)
{
    if(mtar_open(&handle, path.c_str(), "rb") != MTAR_ESUCCESS)
    {
        throw std::filesystem::filesystem_error("During opening tar file " + path.string(), std::error_code{});
    }
    if(mtar_read_header(&handle, &tar_header) != MTAR_ESUCCESS)
    {
        throw std::filesystem::filesystem_error("During reading from tar file " + path.string(), std::error_code{});
    }
}

/**
 * @brief Destroy the entry::entry object
 *
 */
entry::~entry()
{
    mtar_close(&handle);
}

/**
 * @brief size
 *
 * @return std::size_t
 */
std::size_t entry::size() const
{
    return tar_header.size;
}

/**
 * @brief isFile
 *
 * @return true
 * @return false
 */
bool entry::isFile() const
{
    return tar_header.type == MTAR_TREG;
}

/**
 * @brief isDirectory
 *
 * @return true
 * @return false
 */
bool entry::isDirectory() const
{
    return tar_header.type == MTAR_TDIR;
}

/**
 * @brief name
 *
 * @return std::filesystem::path
 */
std::filesystem::path entry::name() const
{
    return tar_header.name;
}

/**
 * @brief read
 *
 * @param data
 * @param size
 */
void entry::read(const std::byte* data, const std::size_t size) const
{
    const std::size_t to_read = size > tar_header.size ? tar_header.size : size;
    if(mtar_read_data(&handle, (void*)(data), to_read) != MTAR_ESUCCESS)
    {
        throw std::filesystem::filesystem_error("During reading from tar file", std::error_code{});
    }
}

/**
 * @brief Construct a new iterator::iterator object
 *
 * @param path
 */
iterator::iterator(const std::filesystem::path& path)
    : entry_{new entry(path)}
{
}

/**
 * @brief operator
 *
 * @return entry&
 */
entry& iterator::operator*() const
{
    return *entry_;
}

/**
 * @brief operator->
 *
 * @return iterator::pointer
 */
iterator::pointer iterator::operator->() const
{
    return &**this;
}

/**
 * @brief operator
 *
 * @return iterator&
 */
iterator& iterator::operator++()
{
    if(mtar_next(&entry_->handle) != MTAR_ESUCCESS or
       mtar_read_header(&entry_->handle, &entry_->tar_header) != MTAR_ESUCCESS)
    {
        entry_ = {};
    }

    return *this;
}

iterator iterator::operator++(int)
{
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

/**
 * @brief operator
 *
 * @param a
 * @param b
 * @return true
 * @return false
 */
bool operator==(const iterator& a, const iterator& b)
{
    return a.entry_ == b.entry_;
}

bool operator!=(const iterator& a, const iterator& b)
{
    return a.entry_ != b.entry_;
}

/**
 * @brief unpack
 *
 * @param path
 * @param where
 */
void unpack(const std::filesystem::path& path, const std::filesystem::path& where)
{
    for(const auto& entry : iterator(path))
    {
        const auto full_path = where / entry.name();
        if(entry.isDirectory() and isValidDirectoryEntry(entry))
        {
            std::filesystem::create_directories(full_path);
        }
        else if(entry.isFile())
        {
            writeToFile(entry, full_path);
        }
    }
}
} // namespace Tar
