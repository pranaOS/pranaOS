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
        C_OBJECT(File)
    public:
        /**
         * @brief Destroy the File object
         * 
         */
        virtual ~File() override;

        /**
         * @param filename 
         * @return ErrorOr<NonnullRefPtr<File>> 
         */
        static ErrorOr<NonnullRefPtr<File>> open(String filename, OpenMode, mode_t = 0644);

        /**
         * @return String 
         */
        String filename() const 
        { 
            return m_filename; 
        }

        /**
         * @brief Set the filename object
         * 
         * @param filename 
         */
        void set_filename(const String filename) 
        { 
            m_filename = move(filename); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_directory() const;

        /**
         * @param filename 
         * @return true 
         * @return false 
         */
        static bool is_directory(String const& filename);

        /**
         * @return true 
         * @return false 
         */
        bool is_device() const;

        /**
         * @param filename 
         * @return true 
         * @return false 
         */
        static bool is_device(String const& filename);

        /**
         * @return true 
         * @return false 
         */
        bool is_link() const;

        /**
         * @param filename 
         * @return true 
         * @return false 
         */
        static bool is_link(String const& filename);

        /**
         * @return true 
         * @return false 
         */
        bool looks_like_shared_library() const;

        /**
         * @param filename 
         * @return true 
         * @return false 
         */
        static bool looks_like_shared_library(String const& filename);

        /**
         * @param filename 
         * @return true 
         * @return false 
         */
        static bool exists(String const& filename);

        /**
         * @param filename 
         * @return ErrorOr<size_t> 
         */
        static ErrorOr<size_t> size(String const& filename);

        /**
         * @return String 
         */
        static String current_working_directory();

        /**
         * @param path 
         * @return String 
         */
        static String absolute_path(String const& path);

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
            PermissionsOwnershipTimestamps,
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

        /**
         * @param filename 
         * @return String 
         */
        static String real_path_for(String const& filename);

        /**
         * @param link_path 
         * @return ErrorOr<String> 
         */
        static ErrorOr<String> read_link(String const& link_path);

        /**
         * @param dst_path 
         * @param src_path 
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> link_file(String const& dst_path, String const& src_path);

        struct RemoveError : public Error 
        {
            /**
             * @brief Construct a new RemoveError object
             * 
             * @param f 
             * @param error_code 
             */
            RemoveError(String f, int error_code)
                : Error(error_code)
                , file(move(f))
            {
            }

            String file;
        }; // struct RemoveError : public Error 

        /**
         * @param path 
         * @param force 
         * @return ErrorOr<void, RemoveError> 
         */
        static ErrorOr<void, RemoveError> remove(String const& path, RecursionMode, bool force);

        /**
         * @return true 
         * @return false 
         */
        virtual bool open(OpenMode) override;

        enum class ShouldCloseFileDescriptor 
        {
            No = 0,
            Yes
        }; // enum class ShouldCloseFileDescriptor 

        /**
         * @param fd 
         * @return true 
         * @return false 
         */
        bool open(int fd, OpenMode, ShouldCloseFileDescriptor);

        /**
         * @return int 
         */
        [[nodiscard]] int leak_fd();

        /**
         * @return NonnullRefPtr<File> 
         */
        static NonnullRefPtr<File> standard_input();
        static NonnullRefPtr<File> standard_output();
        static NonnullRefPtr<File> standard_error();

    private:
        /**
         * @brief Construct a new File object
         * 
         * @param parent 
         */
        File(Object* parent = nullptr)
            : IODevice(parent)
        {
        }

        /**
         * @brief Construct a new File object
         * 
         * @param filename 
         * @param parent 
         */
        explicit File(String filename, Object* parent = nullptr);

        /**
         * @return true 
         * @return false 
         */
        bool open_impl(OpenMode, mode_t);

        String m_filename;
        ShouldCloseFileDescriptor m_should_close_file_descriptor { ShouldCloseFileDescriptor::Yes };
    }; // class File final : public IODevice

} // namespace Core
