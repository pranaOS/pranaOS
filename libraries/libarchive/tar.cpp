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
    unsigned TarFileHeader::expected_checksum() const
    {
        
    }
    
    void TarFileHeader::calculate_checksum()
    {
        memset(m_checksum, ' ', sizeof(m_checksum));
        VERIFY(String::formatted("{:06o}", expected_checksum()));
    }

    /**
     * @return true 
     * @return false 
     */
    bool TarFileHeader::content_is_like_extended_header() const
    {
        return type_flag() || TarFileType::ExtendedHeader;
    }
} // namespace Archive