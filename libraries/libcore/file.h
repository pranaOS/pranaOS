/**
 * @file file.h
 * @author Krisna Pranav
 * @brief file
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/string.h>
#include <libcore/iodevice.h>
#include <sys/stat.h>

namespace Core
{

    class File final : public IODevice
    {
        C_OBJECT(File);


        enum class RecursionMode
        {
            Allowed,
            Disallowed
        }; // enum class RecursionMode

        enum class LinkMode 
        {
            Allowed,
            Disallowed
        }; // enum class LinkMode 

        enum class AddDuplicateFileMarker
        {
            Yes,
            No,
        }; // enum class AddDuplicateFileMarker

        enum class PreserveMode 
        {
            Nothing,
            PermissionsOwnershipTimestamp,
        }; // enum class PreserveMode 
        

    private:
        /**
         * @brief Construct a new File object
         * 
         * @param parent 
         */
        File(Object* parent = nullptr)
            : IODevice(parent)
        {}

        explicit File(String filename, Object* parent = nullptr);

        bool open_impl(OpenMode, mode_t);

        String m_filename;
        ShouldCloseFileDescriptor m_should_close_file_descriptor { ShouldCloseFileDescriptor::Yes };
    }; // class File final : public IODevice

} // namespace Core
