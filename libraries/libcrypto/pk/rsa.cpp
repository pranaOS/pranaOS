/**
 * @file rsa.cpp
 * @author Krisna Pranav
 * @brief rsa
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/debug.h>
#include <mods/random.h>
#include <mods/scopeguard.h>
#include <libcrypto/asn1/asn1.h>
#include <libcrypto/asn1/der.h>
#include <libcrypto/asn1/pem.h>
#include <libcrypto/pk/rsa.h>

namespace Crypto
{

    namespace PK
    {

        /**
         * @param in 
         * @param out 
         */
        void RSA::encrypt(ReadonlyBytes in, Bytes& out)
        {
            dbgln_if(CRYPTO_DEBUG, "in size: {}", in.size());
            auto in_integer = UnsignedBigInteger::import_data(in.data(), in.size());
            if (!(in_integer < m_public_key.modulus())) {
                dbgln("value too large for key");
                out = {};
                return;
            }
            auto exp = NumberTheory::ModularPower(in_integer, m_public_key.public_exponent(), m_public_key.modulus());
            auto size = exp.export_data(out);
            auto outsize = out.size();
            if (size != outsize) {
                dbgln("POSSIBLE RSA BUG!!! Size mismatch: {} requested but {} bytes generated", outsize, size);
                out = out.slice(outsize - size, size);
            }
        }

        /**
         * @param in 
         * @param out 
         */
        void RSA::decrypt(ReadonlyBytes in, Bytes& out)
        {
            auto in_integer = UnsignedBigInteger::import_data(in.data(), in.size());
            auto exp = NumberTheory::ModularPower(in_integer, m_private_key.private_exponent(), m_private_key.modulus());
            auto size = exp.export_data(out);

            auto align = m_private_key.length();
            auto aligned_size = (size + align - 1) / align * align;

            for (auto i = size; i < aligned_size; ++i)
                out[out.size() - i - 1] = 0; 
            out = out.slice(out.size() - aligned_size, aligned_size);
        }

        /**
         * @param in 
         * @param out 
         */
        void RSA::sign(ReadonlyBytes in, Bytes& out)
        {
            auto in_integer = UnsignedBigInteger::import_data(in.data(), in.size());
            auto exp = NumberTheory::ModularPower(in_integer, m_private_key.private_exponent(), m_private_key.modulus());
            auto size = exp.export_data(out);
            out = out.slice(out.size() - size, size);
        }

        /**
         * @param in 
         * @param out 
         */
        void RSA::verify(ReadonlyBytes in, Bytes& out)
        {
            auto in_integer = UnsignedBigInteger::import_data(in.data(), in.size());
            auto exp = NumberTheory::ModularPower(in_integer, m_public_key.public_exponent(), m_public_key.modulus());
            auto size = exp.export_data(out);
            out = out.slice(out.size() - size, size);
        }

        /**
         * @param bytes 
         * @param pem 
         */
        void RSA::import_private_key(ReadonlyBytes bytes, bool pem)
        {
            ByteBuffer buffer;
            if (pem) {
                buffer = decode_pem(bytes);
                bytes = buffer;
            }

            auto key = parse_rsa_key(bytes);
            if (!key.private_key.length()) {
                dbgln("We expected to see a private key, but we found none");
                VERIFY_NOT_REACHED();
            }
            m_private_key = key.private_key;
        }

        /**
         * @param bytes 
         * @param pem 
         */
        void RSA::import_public_key(ReadonlyBytes bytes, bool pem)
        {
            ByteBuffer buffer;
            if (pem) {
                buffer = decode_pem(bytes);
                bytes = buffer;
            }

            auto key = parse_rsa_key(bytes);
            if (!key.public_key.length()) {
                dbgln("We expected to see a public key, but we found none");
                VERIFY_NOT_REACHED();
            }
            m_public_key = key.public_key;
        }

        /**
         * @tparam HashFunction 
         * @param in 
         * @param out 
         */
        template<typename HashFunction>
        void RSA_EMSA_PSS<HashFunction>::sign(ReadonlyBytes in, Bytes& out)
        {
            auto mod_bits = m_rsa.private_key().modulus().trimmed_length() * sizeof(u32) * 8;

            Vector<u8, 2048> EM;
            EM.resize(mod_bits);
            auto EM_buf = Bytes { EM };
            m_emsa_pss.encode(in, EM_buf, mod_bits - 1);

            m_rsa.sign(EM_buf, out);
        }

        /**
         * @tparam HashFunction 
         * @param in 
         * @return VerificationConsistency 
         */
        template<typename HashFunction>
        VerificationConsistency RSA_EMSA_PSS<HashFunction>::verify(ReadonlyBytes in)
        {
            auto mod_bytes = m_rsa.public_key().modulus().trimmed_length() * sizeof(u32);
            if (in.size() != mod_bytes)
                return VerificationConsistency::Inconsistent;

            Vector<u8, 256> EM;
            EM.resize(mod_bytes);
            auto EM_buf = Bytes { EM };

            m_rsa.verify(in, EM_buf);

            return m_emsa_pss.verify(in, EM, mod_bytes * 8 - 1);
        }

        /**
         * @param in 
         * @param out 
         */
        void RSA_PKCS1_EME::encrypt(ReadonlyBytes in, Bytes& out)
        {
            auto mod_len = (m_public_key.modulus().trimmed_length() * sizeof(u32) * 8 + 7) / 8;
            dbgln_if(CRYPTO_DEBUG, "key size: {}", mod_len);
            if (in.size() > mod_len - 11) {
                dbgln("message too long :(");
                out = out.trim(0);
                return;
            }
            if (out.size() < mod_len) {
                dbgln("output buffer too small");
                return;
            }

            auto ps_length = mod_len - in.size() - 3;
            Vector<u8, 8096> ps;
            ps.resize(ps_length);

            fill_with_random(ps.data(), ps_length);
            
            for (size_t i = 0; i < ps_length; ++i)
                while (!ps[i])
                    fill_with_random(ps.span().offset(i), 1);

            u8 paddings[] { 0x00, 0x02 };

            out.overwrite(0, paddings, 2);
            out.overwrite(2, ps.data(), ps_length);
            out.overwrite(2 + ps_length, paddings, 1);
            out.overwrite(3 + ps_length, in.data(), in.size());
            out = out.trim(3 + ps_length + in.size()); 

            dbgln_if(CRYPTO_DEBUG, "padded output size: {} buffer size: {}", 3 + ps_length + in.size(), out.size());

            RSA::encrypt(out, out);
        }

        /**
         * @param in 
         * @param out 
         */
        void RSA_PKCS1_EME::decrypt(ReadonlyBytes in, Bytes& out)
        {
            auto mod_len = (m_public_key.modulus().trimmed_length() * sizeof(u32) * 8 + 7) / 8;
            if (in.size() != mod_len) {
                dbgln("decryption error: wrong amount of data: {}", in.size());
                out = out.trim(0);
                return;
            }

            RSA::decrypt(in, out);

            if (out.size() < RSA::output_size()) {
                dbgln("decryption error: not enough data after decryption: {}", out.size());
                out = out.trim(0);
                return;
            }

            if (out[0] != 0x00) {
                dbgln("invalid padding byte 0 : {}", out[0]);
                return;
            }

            if (out[1] != 0x02) {
                dbgln("invalid padding byte 1 : {}", out[1]);
                return;
            }

            size_t offset = 2;
            while (offset < out.size() && out[offset])
                ++offset;

            if (offset == out.size()) {
                dbgln("garbage data, no zero to split padding");
                return;
            }

            ++offset;

            if (offset - 3 < 8) {
                dbgln("PS too small");
                return;
            }

            out = out.slice(offset, out.size() - offset);
        }

        void RSA_PKCS1_EME::sign(ReadonlyBytes, Bytes&)
        {
            dbgln("FIXME: RSA_PKCS_EME::sign");
        }
        
        void RSA_PKCS1_EME::verify(ReadonlyBytes, Bytes&)
        {
            dbgln("FIXME: RSA_PKCS_EME::verify");
        }

    } // namespace PK

} // namespace Crypto
