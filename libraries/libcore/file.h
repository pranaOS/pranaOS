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

        struct CopyError : public Error
        {
            /**
             * @brief Construct a new CopyError object
             * 
             * @param error_code 
             * @param t 
             */
            CopyError(int error_code, bool t)
                : Error(error_code)
                , tried_recursing(t)
            {
            }

            bool tried_recursing;
        }; // struct CopyError : public Error

        /**
         * @param dst_path 
         * @param src_stat 
         * @param source 
         * @return ErrorOr<void, CopyError> 
         */
        static ErrorOr<void, CopyError> copy_file(String const& dst_path, struct stat const& src_stat, File& source, PreserveMode = PreserveMode::Nothing);

        /**
         * @param dst_path 
         * @param src_path 
         * @param src_stat 
         * @return ErrorOr<void, CopyError> 
         */
        static ErrorOr<void, CopyError> copy_directory(String const& dst_path, String const& src_path, struct stat const& src_stat, LinkMode = LinkMode::Disallowed, PreserveMode = PreserveMode::Nothing);

        /**
         * @param dst_path 
         * @param src_path 
         * @return ErrorOr<void, CopyError> 
         */
        static ErrorOr<void, CopyError> copy_file_or_directory(String const& dst_path, String const& src_path, RecursionMode = RecursionMode::Allowed, LinkMode = LinkMode::Disallowed, AddDuplicateFileMarker = AddDuplicateFileMarker::Yes, PreserveMode = PreserveMode::Nothing);

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
