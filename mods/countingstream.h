/**
 * @file countingstream.h
 * @author Krisna Pranav
 * @brief couting stream
 * @version 6.0
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2021-2025 Patrick, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/maybeowned.h>
#include <mods/stream.h>

namespace Mods 
{

    class CountingStream : public Stream 
    {
    public:
        /**
         * @brief Construct a new Counting Stream object
         * 
         */
        CountingStream(MaybeOwned<Stream>);

        /**
         * @return u64 
         */
        u64 read_bytes() const;

        /**
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read_some(Bytes) override;

        /**
         * @param discarded_bytes 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> discard(size_t discarded_bytes) override;

        /**
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write_some(ReadonlyBytes) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override;
        virtual bool is_open() const override;

        virtual void close() override;

    private:
        MaybeOwned<Stream> m_stream;
        u64 m_read_bytes { 0 };
    }; // class CountingStream : public Stream 

} // namespace Mods
