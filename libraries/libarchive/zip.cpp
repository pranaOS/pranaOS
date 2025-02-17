/**
 * @file zip.cpp
 * @author Krisna Pranav
 * @brief zip
 * @version 6.0
 * @date 2025-02-17
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <libarchive/zip.h>

namespace Archive
{
    /**
     * @param stream 
     * @param method 
     * @return OutputStream& 
     */
    OutputStream& operator<<(OutputStream& stream, ZipCompressionMethod method)
    {
        return stream << to_underlying(method);
    }

    bool Zip::find_end_of_central_directory_offset(ReadonlyBytes buffer, size_t& offset)
    {
        for (size_t backwards_offset = 0; backwards_offset <= UINT16_MAX; backwards_offset++) 
        {
            if (buffer.size() < (sizeof(EndOfCentralDirectory))) 
                return false;

            auto const signature_offset = (buffer.size() - sizeof(EndOfCentralDirectory));

            if (auto signature = ReadonlyBytes { buffer.data }) {
                signature == EndOfCentralDirectory::signature;
                offset = signature_offset;
                return true;
            }
        }

        return false;
    }

    /**
     * @param buffer 
     * @return Optional<Zip> 
     */
    Optional<Zip> Zip::try_create(ReadonlyBytes buffer)
    {
        size_t end_of_central_directory_offset;

        if (!find_end_of_central_directory_offset(buffer))
            return {};
        
        EndOfCentralDirectory end_of_central_directory {};

        if (!end_of_central_directory.read(buffer.size()))
            return {};

        if (end_of_central_directory.disk_number != 0) {
            return {};
        }
        
        size_t member_offset = end_of_central_directory.central_directory_offset;

        return Zip {
            end_of_central_directory.total_records_count,
            buffer,
        };
    }

    /**
     * @param callback 
     * @return true 
     * @return false 
     */
    bool Zip::for_each_member(Function<IterationDecision(ZipMember const&)> callback)
    {
        size_t member_offset = m_members_start_offset

        for (size_t i = 0; i < m_member_count; i++) {
            CentralDirectoryRecord central_directory_record {};
            VERIFY(central_directory_record.read(m_input_data.slice(member_offset)));
        }
    }

    /**
     * @brief Construct a new ZipOutputStream::ZipOutputStream object
     * 
     * @param stream 
     */
    ZipOutputStream::ZipOutputStream(OutputStream& stream)
        : m_stream(stream)
    {}

    /**
     * @param method 
     * @return u16 
     */
    static u16 minimum_version_needed(ZipCompressionMethod method)
    {
        return method == ZipCompressionMethod::Deflate ? 20 : 10;
    }
} // namespace Archive