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
#include <mods/variant.h>
#include <libcrypto/hash/hashfunction.h>
#include <libcrypto/hash/md5.h>
#include <libcrypto/hash/sha1.h>
#include <libcrypto/hash/sha2.h>

namespace Crypto 
{

    namespace Hash 
    {

        enum class HashKind {
            None,
            SHA1,
            SHA256,
            SHA384,
            SHA512,
            MD5,
        }; // enum class HashKind

        struct MultiHashDigestVariant {
            constexpr static size_t Size = 0;

            /**
             * @brief Construct a new MultiHashDigestVariant object
             * 
             * @param digest 
             */
            MultiHashDigestVariant(Empty digest)
                : m_digest(move(digest))
            {
            }

            /**
             * @brief Construct a new MultiHashDigestVariant object
             * 
             * @param digest 
             */
            MultiHashDigestVariant(MD5::DigestType digest)
                : m_digest(move(digest))
            {
            }

            /**
             * @brief Construct a new MultiHashDigestVariant object
             * 
             * @param digest 
             */
            MultiHashDigestVariant(SHA1::DigestType digest)
                : m_digest(move(digest))
            {
            }

            /**
             * @brief Construct a new MultiHashDigestVariant object
             * 
             * @param digest 
             */
            MultiHashDigestVariant(SHA256::DigestType digest)
                : m_digest(move(digest))
            {
            }

            /**
             * @brief Construct a new MultiHashDigestVariant object
             * 
             * @param digest 
             */
            MultiHashDigestVariant(SHA384::DigestType digest)
                : m_digest(move(digest))
            {
            }

            /**
             * @brief Construct a new MultiHashDigestVariant object
             * 
             * @param digest 
             */
            MultiHashDigestVariant(SHA512::DigestType digest)
                : m_digest(move(digest))
            {
            }

            /**
             * @return u8 const* 
             */
            [[nodiscard]] u8 const* immutable_data() const
            {
                return m_digest.visit(
                    [&](Empty const&) -> u8 const* { VERIFY_NOT_REACHED(); },
                    [&](auto const& value) { return value.immutable_data(); });
            }

            /**
             * @return size_t 
             */
            [[nodiscard]] size_t data_length() const
            {
                return m_digest.visit(
                    [&](Empty const&) -> size_t { VERIFY_NOT_REACHED(); },
                    [&](auto const& value) { return value.data_length(); });
            }

            /**
             * @return ReadonlyBytes 
             */
            [[nodiscard]] ReadonlyBytes bytes() const
            {
                return m_digest.visit(
                    [&](Empty const&) -> ReadonlyBytes { VERIFY_NOT_REACHED(); },
                    [&](auto const& value) { return value.bytes(); });
            }

            using DigestVariant = Variant<Empty, MD5::DigestType, SHA1::DigestType, SHA256::DigestType, SHA384::DigestType, SHA512::DigestType>;
            DigestVariant m_digest {};
        }; // struct MultiHashDigestVariant

        class Manager final : public HashFunction<0, 0, MultiHashDigestVariant> 
        {
        public:
            using HashFunction::update;

            /**
             * @brief Construct a new Manager object
             * 
             */
            Manager()
            {
                m_pre_init_buffer = ByteBuffer();
            }

            /**
             * @brief Construct a new Manager object
             * 
             * @param other 
             */
            Manager(Manager const& other) 
            {
                m_pre_init_buffer = ByteBuffer();
                initialize(other.m_kind);
            }

            /**
             * @brief Construct a new Manager object
             * 
             * @param kind 
             */
            Manager(HashKind kind)
            {
                m_pre_init_buffer = ByteBuffer();
                initialize(kind);
            }

            /**
             * @brief Destroy the Manager object
             * 
             */
            ~Manager()
            {
                m_algorithm = Empty {};
            }

            /**
             * @return size_t 
             */
            inline size_t digest_size() const
            {
                return m_algorithm.visit(
                    [&](Empty const&) -> size_t { return 0; },
                    [&](auto const& hash) { return hash.digest_size(); });
            }

            /**
             * @return size_t 
             */
            inline size_t block_size() const
            {
                return m_algorithm.visit(
                    [&](Empty const&) -> size_t { return 0; },
                    [&](auto const& hash) { return hash.block_size(); });
            }

            /**
             * @param kind 
             */
            inline void initialize(HashKind kind)
            {
                if (!m_algorithm.has<Empty>()) {
                    VERIFY_NOT_REACHED();
                }

                m_kind = kind;
                switch (kind) {
                case HashKind::MD5:
                    m_algorithm = MD5();
                    break;
                case HashKind::SHA1:
                    m_algorithm = SHA1();
                    break;
                case HashKind::SHA256:
                    m_algorithm = SHA256();
                    break;
                case HashKind::SHA384:
                    m_algorithm = SHA384();
                    break;
                case HashKind::SHA512:
                    m_algorithm = SHA512();
                    break;
                default:
                case HashKind::None:
                    m_algorithm = Empty {};
                    break;
                }
            }

            /**
             * @param data 
             * @param length 
             */
            virtual void update(u8 const* data, size_t length) override
            {
                auto size = m_pre_init_buffer.size();
                if (size) {
                    m_algorithm.visit(
                        [&](Empty&) {},
                        [&](auto& hash) { hash.update(m_pre_init_buffer); });
                }
                m_algorithm.visit(
                    [&](Empty&) { m_pre_init_buffer.append(data, length); },
                    [&](auto& hash) { hash.update(data, length); });
                if (size && m_kind != HashKind::None)
                    m_pre_init_buffer.clear();
            }

            /**
             * @return DigestType 
             */
            virtual DigestType peek() override
            {
                return m_algorithm.visit(
                    [&](Empty&) -> DigestType { VERIFY_NOT_REACHED(); },
                    [&](auto& hash) -> DigestType { return hash.peek(); });
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

            virtual void reset() override
            {
                m_pre_init_buffer.clear();
                m_algorithm.visit(
                    [&](Empty&) {},
                    [&](auto& hash) { hash.reset(); });
            }

        #ifndef KERNEL
            virtual String class_name() const override
            {
                return m_algorithm.visit(
                    [&](Empty const&) -> String { return "UninitializedHashManager"; },
                    [&](auto const& hash) { return hash.class_name(); });
            }
        #endif

            inline HashKind kind() const
            {
                return m_kind;
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
            using AlgorithmVariant = Variant<Empty, MD5, SHA1, SHA256, SHA384, SHA512>;
            AlgorithmVariant m_algorithm {};
            HashKind m_kind { HashKind::None };
            ByteBuffer m_pre_init_buffer;
        }; // class Manager final : public HashFunction<0, 0, MultiHashDigestVariant> 

    } // namespace Hash

} // namespace Crypto 
