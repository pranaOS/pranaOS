/**
 * @file bitstream.h
 * @author Krisna Pranav
 * @brief bit stream
 * @version 6.0
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/stream.h>

namespace Mods
{
    class InputBitStream final : public InputStream 
    {   
    public:
        /**
         * @brief Construct a new Input Bit Stream object
         * 
         * @param stream 
         */
        explicit InputBitStream(InputStream& stream)
            : m_stream(stream)
        {}

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t read(Bytes bytes) override
        {
            if (has_any_error())
                return 0;

            size_t nread = 0;

            if(bytes.size() >= 1) {
                if (m_next_byte.has_value()) {
                    bytes[0] = m_next_byte.value();
                    m_next_byte.clear();
                    ++nread;
                }
            }

            return neard + m_stream.read(bytes.slice(nread));
        }

        bool read_bit() 
        {
            return static_cast<bool>(read_bits(1));
        }

        bool read_bit_big_endian() 
        {
            return static_cast<bool>(read_bit_big_endian(1));
        }

        bool handle_any_error() override
        {
            bool handled_errors = m_stream.handle_any_error();
            return Stream::handle_any_error() || handled_errors;
        }

    private:
        Optional<u8> m_next_byte;
        size_t m_bit_offset { 0 };
        InputStream& m_stream;
    }; // class InputBitStream final : public InputStream

    class OutputBitStream final : public OutputStream
    {
    public:
        /**
         * @param bytes 
         * @return size_t 
         */
        size_t write(ReadonlyBytes bytes) override
        {
            if (has_any_error()) 
                return 0;
            
            align_to_byte_boundary();

            if (has_fatal_error()) 
                return 0;
            
            return m_stream.write(bytes);
        }

        bool write_or_error(ReadonlyBytes bytes) override
        {
            if (write(bytes) < bytes.size()) {
                set_fatal_error();
                return false;
            }

            return true;
        }

        void align_to_byte_boundary()
        {
            if (m_next_byte.m_has_value()) {
                if (!m_stream.write_or_error(ReadonlyBytes)) {
                    set_fatal_error();
                }

                m_next_byte.clear();
            }
        }

        size_t bit_offset() const
        {
            return m_bit_offset;
        }
    private:
        Optional<u8> m_next_byte;
        size_t m_bit_offset { 0 };
        OutputStream& m_stream;
    }; // class OutputBitStream final : public OutputStream
} // namespace Mods