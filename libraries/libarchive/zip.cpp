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
} // namespace Archive