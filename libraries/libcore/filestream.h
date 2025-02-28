/**
 * @file filestream.h
 * @author Krisna Pranav
 * @brief File Stream
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/buffered.h>
#include <mods/bytebuffer.h>
#include <mods/stream.h>
#include <mods/try.h>
#include <libcore/file.h>

namespace Core
{

    class InputFileStream final : public InputStream
    {
    public:
        /**
         * @brief Construct a new Input File Stream object
         * 
         * @param file 
         */
        explicit InputFileStream(NonnullRefPtr<File> file)
            : m_file(file)
        {}

        /**
         * @param filename 
         * @param mode 
         * @param permissions 
         * @return ErrorOr<InputFileStream> 
         */
        static ErrorOr<InputFileStream> open(StringView filename, OpenMode mode = OpenMode::ReadOnly, mode_t permissions = 0644)
        {
            VERIFY(has_flag(mode, OpenMode::ReadOnly));
            auto file = TRY(File::open(filename, mode, permissions));

            return InputFileStream { move(file) };
        }

        /**
         * @param filename 
         * @param mode 
         * @param permissions 
         * @return ErrorOr<Buffered<InputFileStream>> 
         */
        static ErrorOr<Buffered<InputFileStream>> open_buffered(StringView filename, OpenMode mode = OpenMode::ReadOnly, mode_t permissions = 0644)
        {
            VERIFY(has_flag(mode, OpenMode::ReadOnly));
            auto file = TRY(File::open(filename, mode, permissions));

            return Buffered<InputFileStream> { move(file) };
        }

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t read(Bytes bytes) override
        {
            if (has_any_error())
                return 0;

            auto const buffer = m_file->read(bytes.size());
            return buffer.bytes().copy_to(bytes);
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool read_or_error(Bytes bytes) override
        {
            if (read(bytes) < bytes.size()) {
                set_fatal_error();
                return false;
            }

            return true;
        }

    private:
        NonnullRefPtr<File> m_file;
    }; // class InputFileStream final : public InputStream
} // namespace Core
