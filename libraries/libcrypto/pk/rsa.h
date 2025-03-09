/**
 * @file rsa.h
 * @author Krisna Pranav
 * @brief rsa
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/span.h>
#include <libcrypto/bigint/unsignedbiginteger.h>
#include <libcrypto/numbertheory/modularfunctions.h>
#include <libcrypto/pk/code/emsa_pss.h>
#include <libcrypto/pk/pk.h>

namespace Crypto 
{

    namespace PK 
    {

        /**
         * @tparam Integer 
         */
        template<typename Integer = UnsignedBigInteger>
        class RSAPublicKey 
        {
        public:
            /**
             * @brief Construct a new RSAPublicKey object
             * 
             * @param n 
             * @param e 
             */
            RSAPublicKey(Integer n, Integer e)
                : m_modulus(move(n))
                , m_public_exponent(move(e))
                , m_length(m_modulus.trimmed_length() * sizeof(u32))
            {
            }

            /**
             * @brief Construct a new RSAPublicKey object
             * 
             */
            RSAPublicKey()
                : m_modulus(0)
                , m_public_exponent(0)
            {
            }

            /**
             * @return Integer const& 
             */
            Integer const& modulus() const 
            { 
                return m_modulus; 
            }

            /**
             * @return Integer const& 
             */
            Integer const& public_exponent() const 
            { 
                return m_public_exponent; 
            }

            /**
             * @return size_t 
             */
            size_t length() const 
            { 
                return m_length; 
            }

            /**
             * @brief Set the length object
             * 
             * @param length 
             */
            void set_length(size_t length) 
            { 
                m_length = length; 
            }

            /**
             * @param n 
             * @param e 
             */
            void set(Integer n, Integer e)
            {
                m_modulus = move(n);
                m_public_exponent = move(e);
                m_length = (m_modulus.trimmed_length() * sizeof(u32));
            }

        private:
            Integer m_modulus;
            Integer m_public_exponent;
            size_t m_length { 0 };
        }; // class RSAPublicKey

        /**
         * @tparam Integer 
         */
        template<typename Integer = UnsignedBigInteger>
        class RSAPrivateKey 
        {
        public:
            /**
             * @brief Construct a new RSAPrivateKey object
             * 
             * @param n 
             * @param d 
             * @param e 
             */
            RSAPrivateKey(Integer n, Integer d, Integer e)
                : m_modulus(move(n))
                , m_private_exponent(move(d))
                , m_public_exponent(move(e))
                , m_length(m_modulus.trimmed_length() * sizeof(u32))
            {
            }

            /**
             * @brief Construct a new RSAPrivateKey object
             * 
             */
            RSAPrivateKey() = default;

            /**
             * @return Integer const& 
             */
            Integer const& modulus() const 
            { 
                return m_modulus; 
            }

            /**
             * @return Integer const& 
             */
            Integer const& private_exponent() const 
            { 
                return m_private_exponent; 
            }

            /**
             * @return Integer const& 
             */
            Integer const& public_exponent() const 
            { 
                return m_public_exponent; 
            }

            /**
             * @return size_t 
             */
            size_t length() const 
            { 
                return m_length; 
            }

            /**
             * @brief Set the length object
             * 
             * @param length 
             */
            void set_length(size_t length) 
            { 
                m_length = length; 
            }

            /**
             * @param n 
             * @param d 
             * @param e 
             */
            void set(Integer n, Integer d, Integer e)
            {
                m_modulus = move(n);
                m_private_exponent = move(d);
                m_public_exponent = move(e);
                m_length = m_modulus.trimmed_length() * sizeof(u32);
            }

        private:
            Integer m_modulus;
            Integer m_private_exponent;
            Integer m_public_exponent;
            size_t m_length { 0 };
        }; // class RSAPrivateKey

        /**
         * @tparam PubKey 
         * @tparam PrivKey 
         */
        template<typename PubKey, typename PrivKey>
        struct RSAKeyPair {
            PubKey public_key;
            PrivKey private_key;
        }; // struct RSAKeyPair

        using IntegerType = UnsignedBigInteger;
        class RSA : public PKSystem<RSAPrivateKey<IntegerType>, RSAPublicKey<IntegerType>> 
        {
            template<typename T>
            friend class RSA_EMSA_PSS;

        public:
            using KeyPairType = RSAKeyPair<PublicKeyType, PrivateKeyType>;

            /**
             * @param der 
             * @return KeyPairType 
             */
            static KeyPairType parse_rsa_key(ReadonlyBytes der);

            /**
             * @param bits 
             * @return KeyPairType 
             */
            static KeyPairType generate_key_pair(size_t bits = 256)
            {
                IntegerType e { 65537 }; 
                IntegerType p, q;
                IntegerType lambda;

                do {
                    p = NumberTheory::random_big_prime(bits / 2);
                    q = NumberTheory::random_big_prime(bits / 2);
                    lambda = NumberTheory::LCM(p.minus(1), q.minus(1));
                    dbgln("checking combination p={}, q={}, lambda={}", p, q, lambda.length());
                } while (!(NumberTheory::GCD(e, lambda) == 1));

                auto n = p.multiplied_by(q);

                auto d = NumberTheory::ModularInverse(e, lambda);
                dbgln("Your keys are Pub(n={}, e={}) and Priv(n={}, d={})", n, e, n, d);
                RSAKeyPair<PublicKeyType, PrivateKeyType> keys {
                    { n, e },
                    { n, d, e }
                };
                keys.public_key.set_length(bits / 2 / 8);
                keys.private_key.set_length(bits / 2 / 8);
                return keys;
            }

            /**
             * @brief Construct a new RSA object
             * 
             * @param n 
             * @param d 
             * @param e 
             */
            RSA(IntegerType n, IntegerType d, IntegerType e)
            {
                m_public_key.set(n, e);
                m_private_key.set(n, d, e);
            }

            /**
             * @brief Construct a new RSA object
             * 
             * @param pubkey 
             * @param privkey 
             */
            RSA(PublicKeyType& pubkey, PrivateKeyType& privkey)
                : PKSystem<RSAPrivateKey<IntegerType>, RSAPublicKey<IntegerType>>(pubkey, privkey)
            {
            }

            /**
             * @brief Construct a new RSA object
             * 
             * @param publicKeyPEM 
             * @param privateKeyPEM 
             */
            RSA(ByteBuffer const& publicKeyPEM, ByteBuffer const& privateKeyPEM)
            {
                import_public_key(publicKeyPEM);
                import_private_key(privateKeyPEM);
            }

            /**
             * @brief Construct a new RSA object
             * 
             * @param privKeyPEM 
             */
            RSA(StringView privKeyPEM)
            {
                import_private_key(privKeyPEM.bytes());
                m_public_key.set(m_private_key.modulus(), m_private_key.public_exponent());
            }

            /**
             * @brief Construct a new RSA object
             * 
             */
            RSA()
            {
                auto pair = generate_key_pair();
                m_public_key = pair.public_key;
                m_private_key = pair.private_key;
            }

            /**
             * @param in 
             * @param out 
             */
            virtual void encrypt(ReadonlyBytes in, Bytes& out) override;
            virtual void decrypt(ReadonlyBytes in, Bytes& out) override;

            /**
             * @param in 
             * @param out 
             */
            virtual void sign(ReadonlyBytes in, Bytes& out) override;
            virtual void verify(ReadonlyBytes in, Bytes& out) override;

        #ifndef KERNEL
            virtual String class_name() const override
            {
                return "RSA";
            }
        #endif

            /**
             * @brief 
             * 
             * @return size_t 
             */
            virtual size_t output_size() const override
            {
                return m_public_key.length();
            }

            /**
             * @param pem 
             */
            void import_public_key(ReadonlyBytes, bool pem = true);
            void import_private_key(ReadonlyBytes, bool pem = true);

            /**
             * @return PrivateKeyType const& 
             */
            PrivateKeyType const& private_key() const 
            { 
                return m_private_key; 
            }

            /**
             * @return PublicKeyType const& 
             */
            PublicKeyType const& public_key() const 
            { 
                return m_public_key; 
            }
        }; // class RSA : public PKSystem<RSAPrivateKey<IntegerType>, RSAPublicKey<IntegerType>> 

        /**
         * @tparam HashFunction 
         */
        template<typename HashFunction>
        class RSA_EMSA_PSS 
        {
        public:
            /**
             * @brief Construct a new rsa emsa pss object
             * 
             * @param rsa 
             */
            RSA_EMSA_PSS(RSA& rsa)
                : m_rsa(rsa)
            {
            }

            /**
             * @param in 
             * @param out 
             */
            void sign(ReadonlyBytes in, Bytes& out);

            /**
             * @param in 
             * @return VerificationConsistency 
             */
            VerificationConsistency verify(ReadonlyBytes in);

        private:
            EMSA_PSS<HashFunction, HashFunction::DigestSize> m_emsa_pss;
            RSA m_rsa;
        }; // class RSA_EMSA_PSS 

        class RSA_PKCS1_EME : public RSA 
        {
        public:
            /**
             * @brief Construct a new rsa pkcs1 eme object
             * 
             * @tparam Args 
             * @param args 
             */
            template<typename... Args>
            RSA_PKCS1_EME(Args... args)
                : RSA(args...)
            {
            }

            /**
             * @brief Destroy the rsa pkcs1 eme object
             * 
             */
            ~RSA_PKCS1_EME() = default;

            /**
             * @param in 
             * @param out 
             */
            virtual void encrypt(ReadonlyBytes in, Bytes& out) override;
            virtual void decrypt(ReadonlyBytes in, Bytes& out) override;

            virtual void sign(ReadonlyBytes, Bytes&) override;
            virtual void verify(ReadonlyBytes, Bytes&) override;

        #ifndef KERNEL
            virtual String class_name() const override
            {
                return "RSA_PKCS1-EME";
            }
        #endif
            virtual size_t output_size() const override
            {
                return m_public_key.length();
            }
        }; // class RSA_PKCS1_EME : public RSA 

    } // namespace PK

} // namespace Crypto
