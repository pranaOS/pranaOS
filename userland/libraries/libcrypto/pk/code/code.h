/**
 * @file code.h
 * @author Krisna Pranav
 * @brief code
 * @version 6.0
 * @date 2024-11-19
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcrypto/hash/hashfunction.h>
#include <libcrypto/verification.h>

namespace Crypto 
{

    namespace PK 
    {

        /**
         * @tparam HashFunction 
         */
        template<typename HashFunction>
        class Code 
        {
        public:
            /**
             * @brief Construct a new Code object
             * 
             * @tparam Args 
             * @param args 
             */
            template<typename... Args>
            Code(Args... args)
                : m_hasher(args...)
            {
            }

            /**
             * @param in 
             * @param out 
             * @param em_bits 
             */
            virtual void encode(ReadonlyBytes in, ByteBuffer& out, size_t em_bits) = 0;

            /**
             * @param msg 
             * @param emsg 
             * @param em_bits 
             * @return VerificationConsistency 
             */
            virtual VerificationConsistency verify(ReadonlyBytes msg, ReadonlyBytes emsg, size_t em_bits) = 0;

            /**
             * @return HashFunction const& 
             */
            HashFunction const& hasher() const 
            { 
                return m_hasher; 
            }

            /**
             * @return HashFunction& 
             */
            HashFunction& hasher() 
            { 
                return m_hasher; 
            }

        protected:
            virtual ~Code() = default;

            HashFunction m_hasher;
        }; // class Code

    } // namespace PK

} // namespace Crypto
