/**
 * @file circularbuffer.h
 * @author Krisna Pranav
 * @brief circular buffer
 * @version 6.0
 * @date 2025-03-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/error.h>
#include <mods/hashmap.h>
#include <mods/noncopyable.h>
#include <mods/vector.h>

namespace Mods 
{

    class CircularBuffer 
    {
        MODS_MAKE_NONCOPYABLE(CircularBuffer);
        MODS_MAKE_DEFAULT_MOVABLE(CircularBuffer);

    public:
        /**
         * @brief Create a empty object
         * 
         * @param size 
         * @return ErrorOr<CircularBuffer> 
         */
        static ErrorOr<CircularBuffer> create_empty(size_t size);

        /**
         * @brief Create a initialized object
         * 
         * @return ErrorOr<CircularBuffer> 
         */
        static ErrorOr<CircularBuffer> create_initialized(ByteBuffer);

        /**
         * @brief Destroy the Circular Buffer object
         * 
         */
        ~CircularBuffer() = default;

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t write(ReadonlyBytes bytes);

        /**
         * @param bytes 
         * @return Bytes 
         */
        Bytes read(Bytes bytes);

        /**
         * @param discarded_bytes 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> discard(size_t discarded_bytes);

        /**
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> fill_from_stream(Stream&);

        /**
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> flush_to_stream(Stream&);

        /**
         * @param bytes 
         * @param distance 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read_with_seekback(Bytes bytes, size_t distance) const;

        /**
         * @param distance 
         * @param length 
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> copy_from_seekback(size_t distance, size_t length);

        /**
         * @return size_t 
         */
        [[nodiscard]] size_t empty_space() const;
        [[nodiscard]] size_t used_space() const;
        [[nodiscard]] size_t capacity() const;
        [[nodiscard]] size_t seekback_limit() const;

        /**
         * @param needle 
         * @param from 
         * @param until 
         * @return Optional<size_t> 
         */
        Optional<size_t> offset_of(StringView needle, Optional<size_t> from = {}, Optional<size_t> until = {}) const;

        void clear();

    protected:
        CircularBuffer(ByteBuffer);

        [[nodiscard]] bool is_wrapping_around() const;

        [[nodiscard]] Bytes next_write_span();

        /**
         * @param offset 
         * @return ReadonlyBytes 
         */
        [[nodiscard]] ReadonlyBytes next_read_span(size_t offset = 0) const;

        /**
         * @param distance 
         * @return ReadonlyBytes 
         */
        [[nodiscard]] ReadonlyBytes next_seekback_span(size_t distance) const;

        ByteBuffer m_buffer {};

        size_t m_reading_head {};
        size_t m_used_space {};
        size_t m_seekback_limit {};
    }; // class CircularBuffer

    class SearchableCircularBuffer : public CircularBuffer 
    {
    public:
        /**
         * @brief Create a empty object
         * 
         * @param size 
         * @return ErrorOr<SearchableCircularBuffer> 
         */
        static ErrorOr<SearchableCircularBuffer> create_empty(size_t size);

        /**
         * @brief Create a initialized object
         * 
         * @return ErrorOr<SearchableCircularBuffer> 
         */
        static ErrorOr<SearchableCircularBuffer> create_initialized(ByteBuffer);

        [[nodiscard]] size_t search_limit() const;

        /**
         * @param bytes 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read(Bytes bytes);

        /**
         * @param discarded_bytes 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> discard(size_t discarded_bytes);

        /**
         * @param stream 
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> flush_to_stream(Stream& stream);

        struct Match {
            size_t distance;
            size_t length;
        }; // struct Match
        
        /**
         * @param maximum_length 
         * @param minimum_length 
         * @return Optional<Match> 
         */
        Optional<Match> find_copy_in_seekback(size_t maximum_length, size_t minimum_length = 2);

        /**
         * @param distances 
         * @param maximum_length 
         * @param minimum_length 
         * @return Optional<Match> 
         */
        Optional<Match> find_copy_in_seekback(ReadonlySpan<size_t> distances, size_t maximum_length, size_t minimum_length = 2) const;

        static constexpr size_t HASH_CHUNK_SIZE = 3;

    private:
        /**
         * @param distance 
         * @return ReadonlyBytes 
         */
        [[nodiscard]] ReadonlyBytes next_search_span(size_t distance) const;

        SearchableCircularBuffer(ByteBuffer);

        HashMap<unsigned, size_t> m_hash_location_map;
        HashMap<size_t, size_t> m_location_chain_map;

        ErrorOr<void> insert_location_hash(ReadonlyBytes value, size_t raw_offset);
        ErrorOr<void> hash_last_bytes(size_t count);
    }; // class SearchableCircularBuffer

} // namespace Mods
