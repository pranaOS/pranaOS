/**
 * @file zip.h
 * @author Krisna Pranav
 * @brief zip
 * @version 6.0
 * @date 2025-02-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/function.h>
#include <mods/iterationdecision.h>
#include <mods/stream.h>
#include <mods/string.h>
#include <mods/vector.h>
#include <string.h>

namespace Archive
{
    /**
     * @tparam fields_size 
     * @tparam T 
     * @param buffer 
     * @param self 
     * @return true 
     * @return false 
     */
    template<size_t fields_size, class T>
    static bool read_helper(ReadonlyBytes buffer, T* self)
    {
        if (buffer.size() < T::signature.size() + fields_size)
            return false;
        
        if (buffer.slice(0, T::signature.size()) != T::signature)
            return false;
        
        memcpy(self, buffer.data() + T::signature.size(), fields_size);

        return true;
    }

    static constexpr auto signature_length = 4;

    struct [[gnu::packed]] EndOfCentralDirectory
    {
        static constexpr Array<u8, signature_length> signature = { };

        u16 disk_number;
        u16 central_directory_start_disk;
        u16 disk_records_count;
        u16 total_records_count;
        u32 central_directory_size;
        u32 central_directory_offset;
        u16 comment_length;
        u8 const* comment;

        /**
         * @param buffer 
         * @return true 
         * @return false 
         */
        bool read(ReadonlyBytes buffer)
        {
            constexpr auto fields_size = sizeof(EndOfCurrentDirectory);

            if (!read_helper<fields_size>(buffer, this))
                return false
            
            if (buffer.size() < signature.size() + fields_size + comment_length)
                return false;

            comment = buffer.data() + signature.size() + fields_size;
            return true;
        }
    }; // struct [[gnu::packed]] EndOfCentralDirectory

    enum class ZipCompressionMethod : u16
    {
        Store = 0,
        Shrink = 1,
        Reduce1 = 2,
        Reduce2 = 3,
        Reduce3 = 4,
        Reduce4 = 5,
        Implode = 6,
        Reserved = 7,
        Deflate = 8
    }; // enum class ZipCompressionMethod : u16

    /**
     * @param stream 
     * @param method 
     * @return OutputStream& 
     */
    OutputStream& operator<<(OutputStream& stream, ZipCompressionMethod method);

    struct ZipMember
    {
        String name;
        ReadonlyBytes compressed_data;
        ZipCompressionMethod compression_method;
        u32 uncompressed_size;
        u32 crc32;
        bool is_directory;
    }; // struct ZipMember

    class Zip
    {
    private:
        static bool find_end_of_central_directory_offset();

        u16 m_member_count { 0 };
        size_t m_members_start_offset { 0 };
        ReadonlyBytes m_input_data; 
    }; // class Zip

    class ZipOutputStream
    {
    public:
        /**
         * @brief Construct a new Zip Output Stream object
         * 
         */
        ZipOutputStream(OutputStream&);

    
        void add_member(ZipMember const&);
        void finish();

    private:
        OutputStream& m_stream;
        Vector<ZipMember> m_members;

        bool m_finished { true };
    }; // class ZipOutputStream
} // namespace Archive