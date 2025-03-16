/**
 * @file countingstream.cpp
 * @author Krisna Pranav
 * @brief counting stream
 * @version 6.0
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2021-2025 Patrick, pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/countingstream.h>

namespace Mods 
{

    /**
     * @brief Construct a new CountingStream::CountingStream object
     * 
     * @param stream 
     */
    CountingStream::CountingStream(MaybeOwned<Stream> stream)
        : m_stream(move(stream))
    {
    }

    /**
     * @return u64 
     */
    u64 CountingStream::read_bytes() const
    {
        return m_read_bytes;
    }

    /**
     * @param bytes 
     * @return ErrorOr<Bytes> 
     */
    ErrorOr<Bytes> CountingStream::read_some(Bytes bytes)
    {
        auto result = TRY(m_stream->read_some(bytes));

        m_read_bytes += result.size();

        return result;
    }

    /**
     * @param discarded_bytes 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> CountingStream::discard(size_t discarded_bytes)
    {
        TRY(m_stream->discard(discarded_bytes));

        m_read_bytes += discarded_bytes;

        return {};
    }

    /**
     * @param bytes 
     * @return ErrorOr<size_t> 
     */
    ErrorOr<size_t> CountingStream::write_some(ReadonlyBytes bytes)
    {
        return m_stream->write_some(bytes);
    }

    /**
     * @return true 
     * @return false 
     */
    bool CountingStream::is_eof() const
    {
        return m_stream->is_eof();
    }

    bool CountingStream::is_open() const
    {
        return m_stream->is_open();
    }

    void CountingStream::close()
    {
    }
 
} // namespace Mods
