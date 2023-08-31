/**
 * @file hashmanager.h
 * @author Krisna Pranav
 * @brief hash manager
 * @version 6.0
 * @date 2023-08-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/optional.h>
#include <mods/ownptr.h>
#include <libcrypto/hash/hashfunction.h>
#include <libcrypto/hash/md5.h>
#include <libcrypto/hash/sha1.h>
#include <libcrypto/hash/sha2.h>

namespace Crypto
{
    namespace Hash
    {
        enum class HashKind
        {
            None,
            SHA1,
            SHA256,
            SHA512,
            MD5,
        };

        struct MultiHashDigestVariant
        {
            constexpr static size_t Size = 0;

            MultiHashDigestVariant(SHA1::DigestType digest)
                : sha1(digest)
                , kind(HashKinda::SHA1)
            {}

            const u8* immutable_data() const
            {
                switch(kind) {
                    case HashKind::MD5:
                        return md5.value().immutable_data();
                    case HashKind::SHA1:
                        return sha1.value().immutable_data();
                    case HashKind::SHA256:
                        return sha256.value().immutable_data();
                    default:
                        break;
                }
            }

            size_t data_length()
            {
                switch (kind) {
                    case HashKind::MD5:
                        return md5.value().data_length();
                    case HashKind::SHA1:
                        return sha1.value().data_length();
                };
            }
        }; 

        class Manager final : public HashFunction<0, MultiHashDigestVariant>
        {
        public:
            /// @brief Construct a new Manager object
            Manager()
            {
                m_pre_init_buffer = ByteBuffer::create_zeroed(0);
            }

            /**
             * @param other 
             */
            Manager(const Manager& other)
            {
                m_pre_init_buffer = ByteBuffer::create_zeroed(0);
                initialize(other.m_kind);
            }

            /**
             * @param kind 
             */
            Manager(HashKind kind)
            {
                m_pre_init_buffer = ByteBuffer::create_zeroed(0);
                initialize(kind);
            }
        

        private:
            OwnPtr<SHA1> m_sha1;
            OwnPtr<SHA256> m_sha256;
            OwnPtr<SHA512> m_sha512;
            OwnPtr<MD5> m_md5;
            HashKind m_kind { HashKind::None };
            ByteBuffer m_pre_init_buffer; 
        }; // class Manager
    }
}