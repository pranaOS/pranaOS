/**
 * @file filebackedfilesystem.h
 * @author Krisna Pranav
 * @brief File Backed File System
 * @version 6.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/filesystem/filedescription.h>
#include <kernel/filesystem/filesystem.h>

namespace Kernel 
{

    class FileBackedFS : public FS 
    {
    public:
        virtual ~FileBackedFS() override;

        /**
         * @return File& 
         */
        File& file() 
        { 
            return m_file_description->file(); 
        }

        /**
         * @return FileDescription& 
         */
        FileDescription& file_description() 
        { 
            return *m_file_description; 
        }

        /**
         * @return const File& 
         */
        const File& file() const 
        { 
            return m_file_description->file(); 
        }

        /**
         * @return FileDescription& 
         */
        FileDescription& file_description() const 
        { 
            return *m_file_description; 
        }

    protected:
        /// @brief Construct a new File Backed F S object
        explicit FileBackedFS(FileDescription&);

    private:
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_file_backed() const override 
        { 
            return true; 
        }

        mutable NonnullRefPtr<FileDescription> m_file_description;
    }; // class FileBackedFS

} // namespace Kernel
