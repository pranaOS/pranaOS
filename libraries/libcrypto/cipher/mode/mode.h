/**
 * @file mode.h
 * @author Krisna Pranav
 * @brief mode
 * @version 6.0
 * @date 2023-08-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/byte_buffer.h>
#include <mods/span.h>
#include <mods/stdlibextra.h>
#include <libcrypto/cipher/cipher.h>

namespace Crypto 
{
    namespace Cipher
    {
        template<typename T>
        class Mode
        {
        public:

            /// @brief Destroy the Mode object
            virtual ~Mode() {}
            
            /**
             * @param in 
             * @param out 
             * @param ivec 
             * @param ivec_out 
             */
            virtual void encrypt(const ReadonlyBytes& in, Bytes& out, const Bytes& ivec = {}, Bytes* ivec_out = nullptr) = 0;
            
            /**
             * @param in 
             * @param out 
             * @param ivec 
             */
            virtual void decrypt(const ReadonlyBytes& in, Bytes& out, const Bytes& ivec = {}) = 0;

            virtual size_t IV_length() const = 0;

            /**
             * @return const T& 
             */
            const T& cipher() const
            {
                return m_cipher;
            }

            /**
             * @param input_size 
             * @return ByteBuffer 
             */
            ByteBuffer create_aligned_buffer(size_t input_size) const
            {
                size_t remainder = (input_size + T::block_size());

                if (remainder == 0)
                    return ByteBuffer::create_uninitialized(input_size);
                else
                    return ByteBuffer::create_uninitialized(input_size + T);
            }

            /**
             * @return String 
             */
            virtual String class_name() const = 0;

            /**
             * @return T& 
             */
            T& cipher() 
            {
                return m_cipher;
            }

        private:
            T m_cipher;
        }; // clas Mode
    }
}
