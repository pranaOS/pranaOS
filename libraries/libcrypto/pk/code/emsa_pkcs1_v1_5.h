/**
 * @file emsa_pkcs1_v1_5.h
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */



#pragma once

#include <libcrypto/hash/hashmanager.h>
#include <libcrypto/hash/md5.h>
#include <libcrypto/hash/sha1.h>
#include <libcrypto/hash/sha2.h>
#include <libcrypto/pk/code/code.h>

namespace Crypto 
{

    namespace PK 
    {

        /**
         * @tparam HashFunction 
         */
        template<typename HashFunction>
        class EMSA_PKCS1_V1_5 : public Code<HashFunction> 
        {
        public:
            /**
             * @brief Construct a new emsa pkcs1 v1 5 object
             * 
             * @tparam Args 
             * @param args 
             */
            template<typename... Args>
            EMSA_PKCS1_V1_5(Args... args)
                : Code<HashFunction>(args...)
            {
            }

            /**
             * @param in 
             * @param out 
             * @param em_bits 
             */
            virtual void encode(ReadonlyBytes in, ByteBuffer& out, size_t em_bits) override
            {
                auto& hash_fn = this->hasher();
                hash_fn.update(in);
                auto message_digest = hash_fn.digest();
                auto message_digest_size = message_digest.bytes().size();

                auto digest_info = hash_function_digest_info();
                auto encoded_message_length = digest_info.size() + message_digest_size;

                auto em_bytes = (em_bits + 7) / 8;

                if (em_bytes < encoded_message_length + 11) {
                    dbgln("EMSA-PKCS1-V1_5-ENCODE: intended encoded message length too short");
                    return;
                }

                auto offset = 0;

                out[offset++] = 0x00;
                out[offset++] = 0x01;
                for (size_t i = 0; i < em_bytes - encoded_message_length - 3; i++)
                    out[offset++] = 0xff;
                out[offset++] = 0x00;

                out.overwrite(offset, digest_info.data(), digest_info.size());
                offset += digest_info.size();
                out.overwrite(offset, message_digest.immutable_data(), message_digest.data_length());
            }

            /**
             * @param msg 
             * @param emsg 
             * @param em_bits 
             * @return VerificationConsistency 
             */
            virtual VerificationConsistency verify(ReadonlyBytes msg, ReadonlyBytes emsg, size_t em_bits) override
            {
                auto em_bytes = (em_bits + 7) / 8;
                auto buffer_result = ByteBuffer::create_uninitialized(em_bytes);
                if (buffer_result.is_error()) {
                    dbgln("EMSA-PKCS1-V1_5-VERIFY: out of memory");
                    return VerificationConsistency::Inconsistent;
                }
                auto buffer = buffer_result.release_value();

                encode(msg, buffer, em_bits);

                if (emsg != buffer) {
                    return VerificationConsistency::Inconsistent;
                }
                return VerificationConsistency::Consistent;
            }

        private:
            inline ReadonlyBytes hash_function_digest_info();
        }; // class EMSA_PKCS1_V1_5

        /**
         * @tparam  
         * @return ReadonlyBytes 
         */
        template<>
        inline ReadonlyBytes EMSA_PKCS1_V1_5<Crypto::Hash::MD5>::hash_function_digest_info()
        {
            return { "\x30\x20\x30\x0c\x06\x08\x2a\x86\x48\x86\xf7\x0d\x02\x05\x05\x00\x04\x10", 18 };
        }

        /**
         * @tparam  
         * @return ReadonlyBytes 
         */
        template<>
        inline ReadonlyBytes EMSA_PKCS1_V1_5<Crypto::Hash::SHA1>::hash_function_digest_info()
        {
            return { "\x30\x21\x30\x09\x06\x05\x2b\x0e\x03\x02\x1a\x05\x00\x04\x14", 15 };
        }

        /**
         * @tparam  
         * @return ReadonlyBytes 
         */
        template<>
        inline ReadonlyBytes EMSA_PKCS1_V1_5<Crypto::Hash::SHA256>::hash_function_digest_info()
        {
            return { "\x30\x31\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01\x05\x00\x04\x20", 19 };
        }

        /**
         * @tparam  
         * @return ReadonlyBytes 
         */
        template<>
        inline ReadonlyBytes EMSA_PKCS1_V1_5<Crypto::Hash::SHA384>::hash_function_digest_info()
        {
            return { "\x30\x41\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x02\x05\x00\x04\x30", 19 };
        }

        /**
         * @tparam  
         * @return ReadonlyBytes 
         */
        template<>
        inline ReadonlyBytes EMSA_PKCS1_V1_5<Crypto::Hash::SHA512>::hash_function_digest_info()
        {
            return { "\x30\x51\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x03\x05\x00\x04\x40", 19 };
        }

        /**
         * @tparam  
         * @return ReadonlyBytes 
         */
        template<>
        inline ReadonlyBytes EMSA_PKCS1_V1_5<Crypto::Hash::Manager>::hash_function_digest_info()
        {
            switch (hasher().kind()) {
            case Hash::HashKind::MD5:
                return { "\x30\x20\x30\x0c\x06\x08\x2a\x86\x48\x86\xf7\x0d\x02\x05\x05\x00\x04\x10", 18 };
            case Hash::HashKind::SHA1:
                return { "\x30\x21\x30\x09\x06\x05\x2b\x0e\x03\x02\x1a\x05\x00\x04\x14", 15 };
            case Hash::HashKind::SHA256:
                return { "\x30\x31\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01\x05\x00\x04\x20", 19 };
            case Hash::HashKind::SHA384:
                return { "\x30\x41\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x02\x05\x00\x04\x30", 19 };
            case Hash::HashKind::SHA512:
                return { "\x30\x51\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x03\x05\x00\x04\x40", 19 };
            case Hash::HashKind::None:
            default:
                VERIFY_NOT_REACHED();
            }
        }

    } // namespace PK

} // namespace Crypto
