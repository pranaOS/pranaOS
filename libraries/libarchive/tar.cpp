/**
 * @file tar.cpp
 * @author Krisna Pranav
 * @brief tar
 * @version 6.0
 * @date 2025-02-17
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include "tar.h"

namespace Archive
{
    /**
     * @return unsigned 
     */
    unsigned TarFileHeader::expected_checksum() const
    {
        auto checksum = 0u;
        u8 const* u8_this = reinterpret_cast<u8 const*>(this);
        u8 const* u8_m_checksum = reinterpret_cast<u8 const*>(&m_checksum);
        
        for(auto i = 0u; i < sizeof(TarFileHeader); ++i)
        {
            if(u8_this + i >= u8_m_checksum && u8_this + i < u8_m_checksum + sizeof(m_checksum))
            {
                checksum += ' ';
            }
            else
            {
                checksum += u8_this[i];
            }
        }

        return checksum;
    }

    void TarFileHeader::calculate_checksum()
    {
        memset(m_checksum, ' ', sizeof(m_checksum));
        VERIFY(String::formatted("{:06o}", expected_checksum()).copy_characters_to_buffer(m_checksum, sizeof(m_checksum)));
    }

    /**
     * @return true 
     * @return false 
     */
    bool TarFileHeader::content_is_like_extended_header() const
    {
        return type_flag() == TarFileType::ExtendedHeader || type_flag() == TarFileType::GlobalExtendedHeader;
    }

} // namespace Archive
