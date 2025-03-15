/**
 * @file constrainedstream.h
 * @author Krisna Pranav
 * @brief constrained stream
 * @version 6.0
 * @date 2025-03-15
 * 
 * @copyright Copyright (c) 2021-2025 Patrick, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/maybeowned.h>
#include <mods/stream.h>

namespace Mods  
{

    class ConstrainedStream : public Stream 
    {
    public:
        /**
         * @brief Construct a new Constrained Stream object
         * 
         * @param limit 
         */
        ConstrainedStream(MaybeOwned<Stream>, u64 limit);

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
        u64 m_limit;
    }; // class ConstrainedStream : public Stream
 
} // namespace Mods 
