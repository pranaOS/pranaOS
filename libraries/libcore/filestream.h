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

        
    private:
        NonnullRefPtr<File> m_file;
    }; // class InputFileStream final : public InputStream
} // namespace Core
