/**
 * @file bufferedhelper.h
 * @author Krisna Pranav
 * @brief buffered helper
 * @version 6.0
 * @date 2025-03-13
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/circularbuffer.h>
#include <mods/ownptr.h>
#include <mods/stream.h>

namespace Mods
{

    /**
     * @tparam T 
     */
    template<typename T>
    concept StreamLike = IsBaseOf<Stream, T>;
    template<typename T>
    concept SeekableStreamLike = IsBaseOf<SeekableStream, T>;

    /**
     * @tparam T 
     */
    template<typename T>
    class BufferedHelper 
    {
        MODS_MAKE_NONCOPYABLE(BufferedHelper);
        MODS_MAKE_DEFAULT_MOVABLE(BufferedHelper);

    public:
        /**
         * @brief Construct a new Buffered Helper object
         * 
         * @tparam U 
         * @param stream 
         * @param buffer 
         */
        template<StreamLike U>
        BufferedHelper(Badge<U>, NonnullOwnPtr<T> stream, CircularBuffer buffer)
            : m_stream(move(stream))
            , m_buffer(move(buffer))
        {
        }

        /**
         * @brief Create a buffered object
         * 
         * @tparam BufferedType 
         * @param stream 
         * @param buffer_size 
         * @return ErrorOr<NonnullOwnPtr<BufferedType<T>>> 
         */
        template<template<typename> typename BufferedType>
        static ErrorOr<NonnullOwnPtr<BufferedType<T>>> create_buffered(NonnullOwnPtr<T> stream, size_t buffer_size)
        {
            if (!buffer_size)
                return Error::from_errno(EINVAL);
            if (!stream->is_open())
                return Error::from_errno(ENOTCONN);

            auto buffer = TRY(CircularBuffer::create_empty(buffer_size));

            return adopt_nonnull_own_or_enomem(new BufferedType<T>(move(stream), move(buffer)));
        }

        /**
         * @return T& 
         */
        T& stream() 
        { 
            return *m_stream; 
        }

        /**
         * @return T const& 
         */
        T const& stream() const 
        { 
            return *m_stream; 
        }

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read(Bytes buffer)
        {
            if (!stream().is_open())
                return Error::from_errno(ENOTCONN);
            if (buffer.is_empty())
                return buffer;

            if (m_buffer.used_space() == 0)
                TRY(populate_read_buffer());

            return m_buffer.read(buffer);
        }

        /**
         * @param buffer 
         * @return ErrorOr<StringView> 
         */
        ErrorOr<StringView> read_line(Bytes buffer)
        {
            return StringView { TRY(read_until(buffer, "\n"sv)) };
        }

        /**
         * @param buffer 
         * @param candidate 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read_until(Bytes buffer, StringView candidate)
        {
            return read_until_any_of(buffer, Array { candidate });
        }

        /**
         * @tparam N 
         * @param buffer 
         * @param candidates 
         * @return ErrorOr<Bytes> 
         */
        template<size_t N>
        ErrorOr<Bytes> read_until_any_of(Bytes buffer, Array<StringView, N> candidates)
        {
            if (!stream().is_open())
                return Error::from_errno(ENOTCONN);

            if (buffer.is_empty())
                return buffer;

            auto const candidate = TRY(find_and_populate_until_any_of(candidates, buffer.size()));

            if (stream().is_eof()) {
                if ((candidate.has_value() && candidate->offset + candidate->size > buffer.size())
                    || (!candidate.has_value() && buffer.size() < m_buffer.used_space())) {
                    return Error::from_errno(EMSGSIZE);
                }
            }

            if (candidate.has_value()) {
                auto const read_bytes = m_buffer.read(buffer.trim(candidate->offset));
                TRY(m_buffer.discard(candidate->size));
                return read_bytes;
            }

            return m_buffer.read(buffer);
        }

        /**
         * @param buffer 
         * @return ErrorOr<StringView> 
         */
        ErrorOr<StringView> read_line_with_resize(ByteBuffer& buffer)
        {
            return StringView { TRY(read_until_with_resize(buffer, "\n"sv)) };
        }

        /**
         * @param buffer 
         * @param candidate 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read_until_with_resize(ByteBuffer& buffer, StringView candidate)
        {
            return read_until_any_of_with_resize(buffer, Array { candidate });
        }

        /**
         * @tparam N 
         * @param buffer 
         * @param candidates 
         * @return ErrorOr<Bytes> 
         */
        template<size_t N>
        ErrorOr<Bytes> read_until_any_of_with_resize(ByteBuffer& buffer, Array<StringView, N> candidates)
        {
            if (!stream().is_open())
                return Error::from_errno(ENOTCONN);

            auto candidate = TRY(find_and_populate_until_any_of(candidates));

            size_t bytes_read_to_user_buffer = 0;
            while (!candidate.has_value()) {
                if (m_buffer.used_space() == 0 && stream().is_eof()) {
                    return buffer.span().trim(bytes_read_to_user_buffer);
                }

                if (buffer.size() - bytes_read_to_user_buffer < m_buffer.used_space()) {
                    TRY(buffer.try_resize(buffer.size() + m_buffer.used_space()));
                }

                bytes_read_to_user_buffer += m_buffer.read(buffer.span().slice(bytes_read_to_user_buffer)).size();
                candidate = TRY(find_and_populate_until_any_of(candidates));
            }

            TRY(buffer.try_resize(bytes_read_to_user_buffer + candidate->offset));
            m_buffer.read(buffer.span().slice(bytes_read_to_user_buffer));
            TRY(m_buffer.discard(candidate->size));
            return buffer.span();
        }

        struct Match {
            size_t offset {};
            size_t size {};
        }; // struct Match

        /**
         * @tparam N 
         * @param candidates 
         * @param max_offset 
         * @return ErrorOr<Optional<Match>> 
         */
        template<size_t N>
        ErrorOr<Optional<Match>> find_and_populate_until_any_of(Array<StringView, N> const& candidates, Optional<size_t> max_offset = {})
        {
            Optional<size_t> longest_candidate;
            for (auto& candidate : candidates) {
                if (candidate.length() >= longest_candidate.value_or(candidate.length()))
                    longest_candidate = candidate.length();
            }

            auto const find_candidates = [this, &candidates, &longest_candidate](size_t min_offset, Optional<size_t> max_offset = {}) -> Optional<Match> {
                auto const corrected_minimum_offset = *longest_candidate > min_offset ? 0 : min_offset - *longest_candidate;
                max_offset = max_offset.value_or(m_buffer.used_space());

                Optional<size_t> longest_match;
                size_t match_size = 0;
                for (auto& candidate : candidates) {
                    auto const result = m_buffer.offset_of(candidate, corrected_minimum_offset, *max_offset);

                    if (result.has_value()) {
                        auto previous_match = longest_match.value_or(*result);
                        if ((previous_match < *result) || (previous_match == *result && match_size < candidate.length())) {
                            longest_match = result;
                            match_size = candidate.length();
                        }
                    }
                }

                if (longest_match.has_value())
                    return Match { *longest_match, match_size };

                return {};
            };

            if (auto first_find = find_candidates(0, max_offset); first_find.has_value())
                return first_find;

            auto last_size = m_buffer.used_space();

            while (m_buffer.used_space() < max_offset.value_or(m_buffer.capacity())) {
                auto const read_bytes = TRY(populate_read_buffer());
                if (read_bytes == 0)
                    break;

                if (auto first_find = find_candidates(last_size, max_offset); first_find.has_value())
                    return first_find;
                last_size = m_buffer.used_space();
            }

            return Optional<Match> {};
        }

        /**
         * @param delimiter 
         * @return ErrorOr<bool> 
         */
        ErrorOr<bool> can_read_up_to_delimiter(ReadonlyBytes delimiter)
        {
            if (stream().is_eof())
                return m_buffer.offset_of(delimiter).has_value();

            auto maybe_match = TRY(find_and_populate_until_any_of(Array { StringView { delimiter } }));
            if (maybe_match.has_value())
                return true;

            return stream().is_eof() && m_buffer.offset_of(delimiter).has_value();
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_eof_with_data_left_over() const
        {
            return stream().is_eof() && m_buffer.used_space() > 0;
        }

        bool is_eof() const
        {
            if (m_buffer.used_space() > 0) {
                return false;
            }

            return stream().is_eof();
        }

        /**
         * @return size_t 
         */
        size_t buffer_size() const
        {
            return m_buffer.capacity();
        }

        size_t buffered_data_size() const
        {
            return m_buffer.used_space();
        }

        void clear_buffer()
        {
            m_buffer.clear();
        }

        /**
         * @param count 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> discard_bytes(size_t count)
        {
            return m_buffer.discard(count);
        }

    private:
        /**
         * @return ErrorOr<size_t> 
         */
        ErrorOr<size_t> populate_read_buffer()
        {
            if (m_buffer.empty_space() == 0)
                return 0;

            size_t nread = 0;

            while (true) {
                auto result = m_buffer.fill_from_stream(stream());

                if (result.is_error()) {
                    if (!result.error().is_errno())
                        return result.release_error();
                    if (result.error().code() == EINTR)
                        continue;
                    if (result.error().code() == EAGAIN)
                        break;
                    return result.release_error();
                }

                nread += result.value();
                break;
            }

            return nread;
        }

        NonnullOwnPtr<T> m_stream;
        CircularBuffer m_buffer;
    }; // class BufferedHelper

    /**
     * @tparam T 
     */
    template<SeekableStreamLike T>
    class InputBufferedSeekable final : public SeekableStream 
    {
        friend BufferedHelper<T>;

    public:
        /**
         * @param stream 
         * @param buffer_size 
         * @return ErrorOr<NonnullOwnPtr<InputBufferedSeekable<T>>> 
         */
        static ErrorOr<NonnullOwnPtr<InputBufferedSeekable<T>>> create(NonnullOwnPtr<T> stream, size_t buffer_size = 16384)
        {
            return BufferedHelper<T>::template create_buffered<InputBufferedSeekable>(move(stream), buffer_size);
        }

        /**
         * @brief Construct a new InputBufferedSeekable object
         * 
         * @param other 
         */
        InputBufferedSeekable(InputBufferedSeekable&& other) = default;

        /**
         * @param other 
         * @return InputBufferedSeekable& 
         */
        InputBufferedSeekable& operator=(InputBufferedSeekable&& other) = default;

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read_some(Bytes buffer) override 
        { 
            return m_helper.read(buffer); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write_some(ReadonlyBytes buffer) override 
        { 
            return m_helper.stream().write_some(buffer); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_eof() const override 
        { 
            return m_helper.is_eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_open() const override 
        { 
            return m_helper.stream().is_open(); 
        }

        virtual void close() override 
        { 
            m_helper.stream().close(); 
        }

        /**
         * @param offset 
         * @param mode 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> seek(i64 offset, SeekMode mode) override
        {
            if (mode == SeekMode::FromCurrentPosition) {
                if (0 <= offset && static_cast<u64>(offset) <= m_helper.buffered_data_size()) {
                    MUST(m_helper.discard_bytes(offset));
                    return TRY(m_helper.stream().tell()) - m_helper.buffered_data_size();
                }

                offset = offset - m_helper.buffered_data_size();
            }

            auto result = TRY(m_helper.stream().seek(offset, mode));
            m_helper.clear_buffer();

            return result;
        }

        /**
         * @param length 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> truncate(size_t length) override
        {
            return m_helper.stream().truncate(length);
        }

        /**
         * @param buffer 
         * @return ErrorOr<StringView> 
         */
        ErrorOr<StringView> read_line(Bytes buffer) 
        { 
            return m_helper.read_line(buffer); 
        }
        
        /**
         * @return ErrorOr<bool> 
         */
        ErrorOr<bool> can_read_line()
        {
            return TRY(m_helper.can_read_up_to_delimiter("\n"sv.bytes())) || m_helper.is_eof_with_data_left_over();
        }

        /**
         * @param buffer 
         * @param candidate 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read_until(Bytes buffer, StringView candidate) 
        { 
            return m_helper.read_until(buffer, candidate); 
        }

        /**
         * @tparam N 
         * @param buffer 
         * @param candidates 
         * @return ErrorOr<Bytes> 
         */
        template<size_t N>
        ErrorOr<Bytes> read_until_any_of(Bytes buffer, Array<StringView, N> candidates) 
        { 
            return m_helper.read_until_any_of(buffer, move(candidates)); 
        }

        /**
         * @param delimiter 
         * @return ErrorOr<bool> 
         */
        ErrorOr<bool> can_read_up_to_delimiter(ReadonlyBytes delimiter) 
        { 
            return m_helper.can_read_up_to_delimiter(delimiter); 
        }

        /**
         * @param buffer 
         * @return ErrorOr<StringView> 
         */
        ErrorOr<StringView> read_line_with_resize(ByteBuffer& buffer) 
        { 
            return m_helper.read_line_with_resize(buffer); 
        }

        /**
         * @param buffer 
         * @param candidate 
         * @return ErrorOr<Bytes> 
         */
        ErrorOr<Bytes> read_until_with_resize(ByteBuffer& buffer, StringView candidate) 
        { 
            return m_helper.read_until_with_resize(buffer, candidate); 
        }

        /**
         * @tparam N 
         * @param buffer 
         * @param candidates 
         * @return ErrorOr<Bytes> 
         */
        template<size_t N>
        ErrorOr<Bytes> read_until_any_of_with_resize(ByteBuffer& buffer, Array<StringView, N> candidates) { return m_helper.read_until_any_of_with_resize(buffer, move(candidates)); }


        /**
         * @return size_t 
         */
        size_t buffer_size() const 
        { 
            return m_helper.buffer_size(); 
        }

        virtual ~InputBufferedSeekable() override = default;

    private:
        /**
         * @brief Construct a new Input Buffered Seekable object
         * 
         * @param stream 
         * @param buffer 
         */
        InputBufferedSeekable(NonnullOwnPtr<T> stream, CircularBuffer buffer)
            : m_helper(Badge<InputBufferedSeekable<T>> {}, move(stream), move(buffer))
        {
        }

        BufferedHelper<T> m_helper;
    }; // class InputBufferedSeekable final : public SeekableStream

    /**
     * @tparam T 
     */
    template<SeekableStreamLike T>
    class OutputBufferedSeekable : public SeekableStream 
    {
    public:
        /**
         * @param stream 
         * @param buffer_size 
         * @return ErrorOr<NonnullOwnPtr<OutputBufferedSeekable<T>>> 
         */
        static ErrorOr<NonnullOwnPtr<OutputBufferedSeekable<T>>> create(NonnullOwnPtr<T> stream, size_t buffer_size = 16 * KiB)
        {
            if (buffer_size == 0)
                return Error::from_errno(EINVAL);
            if (!stream->is_open())
                return Error::from_errno(ENOTCONN);

            auto buffer = TRY(CircularBuffer::create_empty(buffer_size));

            return adopt_nonnull_own_or_enomem(new OutputBufferedSeekable<T>(move(stream), move(buffer)));
        }

        /**
         * @brief Construct a new Output Buffered Seekable object
         * 
         * @param other 
         */
        OutputBufferedSeekable(OutputBufferedSeekable&& other) = default;

        /**
         * @param other 
         * @return OutputBufferedSeekable& 
         */
        OutputBufferedSeekable& operator=(OutputBufferedSeekable&& other) = default;

        /**
         * @param buffer 
         * @return ErrorOr<Bytes> 
         */
        virtual ErrorOr<Bytes> read_some(Bytes buffer) override
        {
            TRY(flush_buffer());
            return m_stream->read_some(buffer);
        }

        /**
         * @param buffer 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> write_some(ReadonlyBytes buffer) override
        {
            if (!m_stream->is_open())
                return Error::from_errno(ENOTCONN);

            auto const written = m_buffer.write(buffer);

            if (m_buffer.empty_space() == 0)
                TRY(m_buffer.flush_to_stream(*m_stream));

            return written;
        }

        virtual bool is_eof() const override
        {
            MUST(flush_buffer());
            return m_stream->is_eof();
        }

        virtual bool is_open() const override 
        { 
            return m_stream->is_open(); 
        }

        virtual void close() override
        {
            MUST(flush_buffer());
            m_stream->close();
        }

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> flush_buffer() const
        {
            while (m_buffer.used_space() > 0)
                TRY(m_buffer.flush_to_stream(*m_stream));
            return {};
        }

        virtual ErrorOr<size_t> tell() const override
        {
            return TRY(m_stream->tell()) + m_buffer.used_space();
        }

        /**
         * @param offset 
         * @param mode 
         * @return ErrorOr<size_t> 
         */
        virtual ErrorOr<size_t> seek(i64 offset, SeekMode mode) override
        {
            TRY(flush_buffer());
            return m_stream->seek(offset, mode);
        }

        /**
         * @param length 
         * @return ErrorOr<void> 
         */
        virtual ErrorOr<void> truncate(size_t length) override
        {
            TRY(flush_buffer());
            return m_stream->truncate(length);
        }

        virtual ~OutputBufferedSeekable() override
        {
            MUST(flush_buffer());
        }

    private:
        /**
         * @brief Construct a new Output Buffered Seekable object
         * 
         * @param stream 
         * @param buffer 
         */
        OutputBufferedSeekable(NonnullOwnPtr<T> stream, CircularBuffer buffer)
            : m_stream(move(stream))
            , m_buffer(move(buffer))
        {
        }

        mutable NonnullOwnPtr<T> m_stream;
        mutable CircularBuffer m_buffer;
    }; // class OutputBufferedSeekable : public SeekableStream

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::BufferedHelper;
using Mods::InputBufferedSeekable;
using Mods::OutputBufferedSeekable;
#endif
