/**
 * @file constrainedstream.cpp
 * @author Krisna Pranav
 * @brief constrained stream
 * @version 6.0
 * @date 2025-03-15
 * 
 * @copyright Copyright (c) 2021-2025 Patrick, pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/constrainedstream.h>

namespace Mods 
{

    /**
     * @brief Construct a new ConstrainedStream::ConstrainedStream object
     * 
     * @param stream 
     * @param limit 
     */
    ConstrainedStream::ConstrainedStream(MaybeOwned<Stream> stream, u64 limit)
        : m_stream(move(stream))
        , m_limit(limit)
    {
    }

    /**
     * @param bytes 
     * @return ErrorOr<Bytes> 
     */
    ErrorOr<Bytes> ConstrainedStream::read_some(Bytes bytes)
    {
        if (bytes.size() >= m_limit)
            bytes = bytes.trim(m_limit);

        auto result = TRY(m_stream->read_some(bytes));

        m_limit -= result.size();

        return result;
    }

    /**
     * @param discarded_bytes 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> ConstrainedStream::discard(size_t discarded_bytes)
    {
        if (discarded_bytes >= m_limit)
            return Error::from_string_literal("Trying to discard more bytes than allowed");

        m_limit -= discarded_bytes;
        TRY(m_stream->discard(discarded_bytes));

        return {};
    }

    /**
     * @return ErrorOr<size_t> 
     */
    ErrorOr<size_t> ConstrainedStream::write_some(ReadonlyBytes)
    {
        return Error::from_errno(EBADF);
    }

    /**
     * @return true 
     * @return false 
     */
    bool ConstrainedStream::is_eof() const
    {
        return m_limit == 0 || m_stream->is_eof();
    }

    /**
     * @return true 
     * @return false 
     */
    bool ConstrainedStream::is_open() const
    {
        return m_stream->is_open();
    }

    void ConstrainedStream::close()
    {
    }

} // namespace Mods
