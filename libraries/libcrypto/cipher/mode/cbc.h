/**
 * @file cbc.h
 * @author Krisna Pranav
 * @brief cbc
 * @version 6.0
 * @date 2023-08-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/string_view.h>
#include <libcrypto/cipher/mode/mode.h>

namespace Crypto
{
    namespace Cipher
    {
        template<typename T>
        class CBC : public Mode<T> 
        {   
        public:
            constexpr static size_t IVSizeInBits = 128;

            /// @brief Destroy the CBC object
            virtual ~CBC() {}
            
            /**
             * @tparam Args 
             */
            template<typename... Args>
            explicit constexpr CBC<T>(Args... args)
                : Mode<T>(args...)
            {}

            /**
             * @return String 
             */
            virtual String class_name() const override
            {
                StringBuilder builder;

                builder.append(this->cipher().class_name());
                builder.append("_CBC");

                return builder.build();
            }

            /**
             * @return size_t 
             */
            virtual size_t IV_length() const override
            {
                return IVSizeInBits / 8;
            }

        private:
            typename T::BlockType m_cipher_block {};
        }; // class CBC
    }
}