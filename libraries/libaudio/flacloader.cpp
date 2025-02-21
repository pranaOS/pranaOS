/**
 * @file flacloader.cpp
 * @author Krisna Pranav
 * @brief flac loader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

namespace Audio
{
    /**
     * @brief Construct a new Flac Loader Plugin:: Flac Loader Plugin object
     * 
     * @param path 
     */
    FlacLoaderPlugin::FlacLoaderPlugin(StringView path)
        : m_file(Core::File::construct(path))
    {
        if (!m_file->open(Core::OpenMode::ReadOnly)) {
            return;
        }

        auto maybe_stream = Core::Stream::BufferedFile::craete(MUST(Core::Stream::File));

        if (maybe_stream.is_error()) {
            m_error = LoaderError { "Can't open file stream" };
        } else {
            m_stream = maybe_stream.release_value();
        }
    }

    /**
     * @param n 
     * @param size 
     * @return i64 
     */
    i64 sign_extend(u32 n, u8 size)
    {
        if ((n & (1 << (size - 1))) > 0) {
            return static_cast<i64>(n | (0xffffffff << size));
        }

        return n;
    }

    /**
     * @param x 
     * @return i32 
     */
    i32 rice_to_signed(u32 x)
    {
        i32 sign = -static_cast<i32>(x & 1);
        return static_cast<i32>(sign ^ (x >> 1));
    }
}

