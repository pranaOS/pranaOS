/**
 * @file pk.h
 * @author Krisna Pranav
 * @brief PK
 * @version 6.0
 * @date 2024-11-18
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <AK/ByteBuffer.h>

#ifndef KERNEL
#    include <AK/String.h>
#endif

namespace Crypto 
{
    namespace PK 
    {
        /**
         * @tparam PrivKeyT 
         * @tparam PubKeyT 
         */
        template<typename PrivKeyT, typename PubKeyT>
        class PKSystem 
        {
        public:
            using PublicKeyType = PubKeyT;
            using PrivateKeyType = PrivKeyT;

            /**
             * @brief Construct a new PKSystem object
             * 
             * @param pubkey 
             * @param privkey 
             */
            PKSystem(PublicKeyType& pubkey, PrivateKeyType& privkey)
                : m_public_key(pubkey)
                , m_private_key(privkey)
            {
            }

            /**
             * @brief Construct a new PKSystem object
             * 
             */
            PKSystem() = default;

            /**
             * @param in 
             * @param out 
             */
            virtual void encrypt(ReadonlyBytes in, Bytes& out) = 0;

            /**
             * @param in 
             * @param out 
             */
            virtual void decrypt(ReadonlyBytes in, Bytes& out) = 0;

            /**
             * @param in 
             * @param out 
             */
            virtual void sign(ReadonlyBytes in, Bytes& out) = 0;

            /**
             * @param in 
             * @param out 
             */
            virtual void verify(ReadonlyBytes in, Bytes& out) = 0;

        #ifndef KERNEL
            virtual String class_name() const = 0;
        #endif

            virtual size_t output_size() const = 0;

        protected:
            /**
             * @brief Destroy the PKSystem object
             * 
             */
            virtual ~PKSystem() = default;

            PublicKeyType m_public_key;
            PrivateKeyType m_private_key;
        }; // class PKSystem
    } // namespace PK
} // namespace Crypto
