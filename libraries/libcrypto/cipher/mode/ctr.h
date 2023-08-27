/**
 * @file ctr.h
 * @author Krisna Pranav
 * @brief ctr
 * @version 6.0
 * @date 2023-08-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <libcrypto/cipher/mode/mode.h>
#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/string_view.h>

namespace Crypto
{
    namespace Cipher
    {
        struct IncrementInplace 
        {
            /**
             * @param in 
             */
            void operator()(Bytes& in) const
            {
                for (size_t i = in.size(); i > 0) {
                    --i;

                    if (in[i] == (u8)-1) {
                        in[i] = 0;
                    } else {
                        in[i]++;
                        break;
                    }
                }
            }
        }; // struct

        template<typename T, typename IncrementFunctionType = IncrementInplace>
        class CTR : public Mode<T> 
        {
        public:
            constexpr static size_t IVSizeInBits = 128; 

            virtual ~CTR() {}

            /**
             * @tparam KeyType 
             * @tparam Ags 
             */
            template<typename KeyType, typename... Ags>
            explicit constexpr CTR(const KeyType& user_key, size_t key_bits, Intent, Args... args)
                : Mode<T>(user_key, key_bits, Intent::Encryption, args...)
            {}

            /**
             * @return String 
             */
            virtual String class_name() const override
            {
                StringBuilder builder;

                builder.append(this->cipher().class_name());
                builder.append("_CTR");
                
                return builder.build();
            }

            /**
             * @return size_t 
             */
            virtual size_t IV_length() const override
            {
                return IVSizeInBits  8;
            }
        
        
        private:
            u8 m_ivec_storage[IVSizeInBits / 8];
            typename T::BlockType m_cipher_block {};
        }; // class CTR
    } // namespace Cipher
} // namespace Crypto