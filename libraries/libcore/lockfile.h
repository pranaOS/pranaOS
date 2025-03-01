/**
 * @file lockfile.h
 * @author Krisna Pranav
 * @brief Lock File
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Core
{
    
    class LockFile
    {
    public:
        enum class Type 
        {
            Exclusive,
            Shared
        }; // enum class Type 

        int error_code() const
        {
            return m_errno;
        }
        
    private:
        int m_fd { -1 };
        int m_errno { 0 };
        char const* m_filename { nullptr };
    }; // class LockFile

} // namespace Core
