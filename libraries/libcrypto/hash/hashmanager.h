/**
 * @file hashmanager.h
 * @author Krisna Pranav
 * @brief hash manager
 * @version 6.0
 * @date 2023-08-31
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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

            /**
             * @param digest 
             */
            MultiHashDigestVariant(SHA1::DigestType digest)
                : sha1(digest)
                , kind(HashKind::SHA1)
            {
            }

            /**
             * @param digest 
             */
            MultiHashDigestVariant(SHA256::DigestType digest)
                : sha256(digest)
                , kind(HashKind::SHA256)
            {
            }

            MultiHashDigestVariant(SHA512::DigestType digest)
                : sha512(digest)
                , kind(HashKind::SHA512)
            {
            }

            /**
             * @param digest 
             */
            MultiHashDigestVariant(MD5::DigestType digest)
                : md5(digest)
                , kind(HashKind::MD5)
            {
            }

            /**
             * @return const u8* 
             */
            const u8* immutable_data() const
            {
                switch (kind) {
                    case HashKind::MD5:
                        return md5.value().immutable_data();
                    case HashKind::SHA1:
                        return sha1.value().immutable_data();
                    case HashKind::SHA256:
                        return sha256.value().immutable_data();
                    case HashKind::SHA512:
                        return sha512.value().immutable_data();
                    default:
                    case HashKind::None:
                        ASSERT_NOT_REACHED();
                        break;
                }
            }

            /**
             * @return size_t 
             */
            size_t data_length()
            {
                switch (kind) {
                    case HashKind::MD5:
                        return md5.value().data_length();
                    case HashKind::SHA1:
                        return sha1.value().data_length();
                    case HashKind::SHA256:
                        return sha256.value().data_length();
                    case HashKind::SHA512:
                        return sha512.value().data_length();
                    default:
                    case HashKind::None:
                        ASSERT_NOT_REACHED();
                        break;
                }
            }

            Optional<SHA1::DigestType> sha1;
            Optional<SHA256::DigestType> sha256;
            Optional<SHA512::DigestType> sha512;
            Optional<MD5::DigestType> md5;
            HashKind kind { HashKind::None };
        }; // struct MultiHashDigestVariant 

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

            /// @brief Destroy the Manager object
            ~Manager()
            {
                m_sha1 = nullptr;
                m_sha256 = nullptr;
                m_sha512 = nullptr;
                m_md5 = nullptr;
            }

            /**
             * @param buffer 
             */
            virtual void update(const ByteBuffer& buffer) override 
            { 
                update(buffer.data(), buffer.size()); 
            };

            /**
             * @param string 
             */
            virtual void update(const StringView& string) override 
            { 
                update((const u8*)string.characters_without_null_termination(), string.length()); 
            };

            /**
             * @return size_t 
             */
            inline size_t digest_size() const
            {
                switch (m_kind) {
                    case HashKind::MD5:
                        return m_md5->digest_size();
                    case HashKind::SHA1:
                        return m_sha1->digest_size();
                    case HashKind::SHA256:
                        return m_sha256->digest_size();
                    case HashKind::SHA512:
                        return m_sha512->digest_size();
                    default:
                    case HashKind::None:
                        return 0;
                }
            }

            /**
             * @return size_t 
             */
            inline size_t block_size() const
            {
                switch (m_kind) {
                    case HashKind::MD5:
                        return m_md5->block_size();
                    case HashKind::SHA1:
                        return m_sha1->block_size();
                    case HashKind::SHA256:
                        return m_sha256->block_size();
                    case HashKind::SHA512:
                        return m_sha512->block_size();
                    default:
                    case HashKind::None:
                        return 0;
                }
            }

            /**
             * 
             * @param kind 
             */
            inline void initialize(HashKind kind)
            {
                if (m_kind != HashKind::None) {
                    ASSERT_NOT_REACHED();
                }

                m_kind = kind;
                switch (kind) {
                    case HashKind::MD5:
                        m_md5 = make<MD5>();
                        break;
                    case HashKind::SHA1:
                        m_sha1 = make<SHA1>();
                        break;
                    case HashKind::SHA256:
                        m_sha256 = make<SHA256>();
                        break;
                    case HashKind::SHA512:
                        m_sha512 = make<SHA512>();
                        break;
                    default:
                    case HashKind::None:
                        break;
                }
            }

            /**
             * @param data 
             * @param length 
             */
            virtual void update(const u8* data, size_t length) override
            {
                auto size = m_pre_init_buffer.size();

                switch (m_kind) {
                case HashKind::MD5:
                        if (size)
                            m_md5->update(m_pre_init_buffer);
                        m_md5->update(data, length);
                        break;
                    case HashKind::SHA1:
                        if (size)
                            m_sha1->update(m_pre_init_buffer);
                        m_sha1->update(data, length);
                        break;
                    case HashKind::SHA256:
                        if (size)
                            m_sha256->update(m_pre_init_buffer);
                        m_sha256->update(data, length);
                        break;
                    case HashKind::SHA512:
                        if (size)
                            m_sha512->update(m_pre_init_buffer);
                        m_sha512->update(data, length);
                        break;
                    default:
                    case HashKind::None:
                        m_pre_init_buffer.append(data, length);
                        return;
                }

                if (size)
                    m_pre_init_buffer.clear();
            }

            /**
             * @return DigestType 
             */
            virtual DigestType peek() override
            {
                switch (m_kind) {
                    case HashKind::MD5:
                        return { m_md5->peek() };
                    case HashKind::SHA1:
                        return { m_sha1->peek() };
                    case HashKind::SHA256:
                        return { m_sha256->peek() };
                    case HashKind::SHA512:
                        return { m_sha512->peek() };
                    default:
                    case HashKind::None:
                        ASSERT_NOT_REACHED();
                        break;
                }
            }

            /**
             * @return DigestType 
             */
            virtual DigestType digest() override
            {
                auto digest = peek();
                reset();
                return digest;
            }

            /// @brief: reset
            virtual void reset() override
            {
                m_pre_init_buffer.clear();
                switch (m_kind) {
                    case HashKind::MD5:
                        m_md5->reset();
                        break;
                    case HashKind::SHA1:
                        m_sha1->reset();
                        break;
                    case HashKind::SHA256:
                        m_sha256->reset();
                        break;
                    case HashKind::SHA512:
                        m_sha512->reset();
                        break;
                    default:
                    case HashKind::None:
                        break;
                }
            }

            /**
             * @return String 
             */
            virtual String class_name() const override
            {
                switch (m_kind) {
                    case HashKind::MD5:
                        return m_md5->class_name();
                    case HashKind::SHA1:
                        return m_sha1->class_name();
                    case HashKind::SHA256:
                        return m_sha256->class_name();
                    case HashKind::SHA512:
                        return m_sha512->class_name();
                    default:
                    case HashKind::None:
                        return "UninitializedHashManager";
                }
            }

            /**
             * @param kind 
             * @return true 
             * @return false 
             */
            inline bool is(HashKind kind) const
            {
                return m_kind == kind;
            }

        private:
            OwnPtr<SHA1> m_sha1;
            OwnPtr<SHA256> m_sha256;
            OwnPtr<SHA512> m_sha512;
            OwnPtr<MD5> m_md5;
            HashKind m_kind { HashKind::None };
            ByteBuffer m_pre_init_buffer;
        }; // class Manager
    } // namespace Hash
} // namespace Crypto
