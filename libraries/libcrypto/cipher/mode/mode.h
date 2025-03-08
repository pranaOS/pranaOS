/**
 * @file mode.h
 * @author Krisna Pranav
 * @brief mode
 * @version 6.0
 * @date 2023-08-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <mods/span.h>
#include <mods/stdlibextra.h>
#include <libcrypto/cipher/cipher.h>

namespace Crypto 
{

    namespace Cipher 
    {
        /**
         * @tparam T 
         */
        template<typename T>
        class Mode 
        {
        public:
            /**
             * @brief Destroy the Mode object
             * 
             */
            virtual ~Mode() = default;

            /**
             * @param in 
             * @param out 
             * @param ivec 
             * @param ivec_out 
             */
            virtual void encrypt(ReadonlyBytes in, Bytes& out, ReadonlyBytes ivec = {}, Bytes* ivec_out = nullptr) = 0;

            /**
             * @param in 
             * @param out 
             * @param ivec 
             */
            virtual void decrypt(ReadonlyBytes in, Bytes& out, ReadonlyBytes ivec = {}) = 0;

            /**
             * @return size_t 
             */
            virtual size_t IV_length() const = 0;

            /**
             * @return const T& 
             */
            const T& cipher() const 
            { 
                return m_cipher; 
            }

            /**
             * @brief Create a aligned buffer object
             * 
             * @param input_size 
             * @return ErrorOr<ByteBuffer> 
             */
            ErrorOr<ByteBuffer> create_aligned_buffer(size_t input_size) const
            {
                size_t remainder = (input_size + T::block_size()) % T::block_size();
                if (remainder == 0)
                    return ByteBuffer::create_uninitialized(input_size);
                else
                    return ByteBuffer::create_uninitialized(input_size + T::block_size() - remainder);
            }

        #ifndef KERNEL
            virtual String class_name() const = 0;
        #endif

            /**
             * @return T& 
             */
            T& cipher()
            {
                return m_cipher;
            }

        protected:
            /**
             * @param data 
             */
            virtual void prune_padding(Bytes& data)
            {
                auto size = data.size();
                switch (m_cipher.padding_mode()) {
                case PaddingMode::CMS: {
                    auto maybe_padding_length = data[size - 1];
                    if (maybe_padding_length >= T::block_size()) {
                        return;
                    }
                    for (auto i = size - maybe_padding_length; i < size; ++i) {
                        if (data[i] != maybe_padding_length) {
                            return;
                        }
                    }
                    data = data.slice(0, size - maybe_padding_length);
                    break;
                }
                case PaddingMode::RFC5246: {
                    auto maybe_padding_length = data[size - 1];
                    for (auto i = size - maybe_padding_length - 1; i < size; ++i) {
                        if (data[i] != maybe_padding_length) {
                            return;
                        }
                    }
                    data = data.slice(0, size - maybe_padding_length - 1);
                    break;
                }
                case PaddingMode::Null: {
                    while (data[size - 1] == 0)
                        --size;
                    data = data.slice(0, size);
                    break;
                }
                default:
                    VERIFY_NOT_REACHED();
                    break;
                }
            }

            /**
             * @brief Construct a new Mode object
             * 
             * @tparam Args 
             * @param args 
             */
            template<typename... Args>
            Mode(Args... args)
                : m_cipher(args...)
            {
            }

        private:
            T m_cipher;
        }; // class Mode

    } // namespace Cipher

} // namespace Crypto
