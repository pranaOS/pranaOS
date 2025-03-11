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

        static constexpr Array<int, 7> pkcs8_rsa_key_oid { 1, 2, 840, 113549, 1, 1, 1 };

        /**
         * @param der 
         * @return RSA::KeyPairType 
         */
        RSA::KeyPairType RSA::parse_rsa_key(ReadonlyBytes der)
        {
            KeyPairType keypair;

            ASN1::Decoder decoder(der);
            
            {
                auto result = decoder.peek();
                if (result.is_error()) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA key parse failed: {}", result.error());
                    return keypair;
                }
                auto tag = result.value();
                if (tag.kind != ASN1::Kind::Sequence) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA key parse failed: Expected a Sequence but got {}", ASN1::kind_name(tag.kind));
                    return keypair;
                }
            }

            {
                auto error = decoder.enter();
                if (error.has_value()) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA key parse failed: {}", error.value());
                    return keypair;
                }
            }

            bool has_read_error = false;

            auto const check_if_pkcs8_rsa_key = [&] {
                auto tag_result = decoder.peek();
                if (tag_result.is_error()) {

                    dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#8 public key parse failed: {}", tag_result.error());
                    return false;
                }

                auto tag = tag_result.value();
                if (tag.kind != ASN1::Kind::Sequence) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#8 public key parse failed: Expected a Sequence but got {}", ASN1::kind_name(tag.kind));
                    return false;
                }

                auto error = decoder.enter();
                if (error.has_value()) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#8 public key parse failed: {}", error.value());
                    return false;
                }

                ScopeGuard leave { [&] {
                    auto error = decoder.leave();
                    if (error.has_value()) {
                        dbgln_if(RSA_PARSE_DEBUG, "RSA key parse failed: {}", error.value());
                        has_read_error = true;
                    }
                } };

                auto oid_result = decoder.read<Vector<int>>();
                if (oid_result.is_error()) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#8 public key parse failed: {}", oid_result.error());
                    return false;
                }

                auto oid = oid_result.release_value();
                if (oid != pkcs8_rsa_key_oid) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#8 public key parse failed: Not an RSA key");
                    return false;
                }

                return true;
            };

            auto integer_result = decoder.read<UnsignedBigInteger>();

            if (!integer_result.is_error()) {
                auto first_integer = integer_result.release_value();

                if (check_if_pkcs8_rsa_key()) {
                    if (has_read_error)
                        return keypair;

                    auto data_result = decoder.read<StringView>();
                    if (data_result.is_error()) {
                        dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#8 private key parse failed: {}", data_result.error());
                        return keypair;
                    }
                    return parse_rsa_key(data_result.value().bytes());
                }

                if (has_read_error)
                    return keypair;

                if (first_integer == 0) {
                    auto modulus_result = decoder.read<UnsignedBigInteger>();
                    if (modulus_result.is_error()) {
                        dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#1 private key parse failed: {}", modulus_result.error());
                        return keypair;
                    }
                    auto modulus = modulus_result.release_value();

                    auto public_exponent_result = decoder.read<UnsignedBigInteger>();
                    if (public_exponent_result.is_error()) {
                        dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#1 private key parse failed: {}", public_exponent_result.error());
                        return keypair;
                    }
                    auto public_exponent = public_exponent_result.release_value();

                    auto private_exponent_result = decoder.read<UnsignedBigInteger>();
                    if (private_exponent_result.is_error()) {
                        dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#1 private key parse failed: {}", private_exponent_result.error());
                        return keypair;
                    }
                    auto private_exponent = private_exponent_result.release_value();

                    keypair.private_key = { modulus, move(private_exponent), public_exponent };
                    keypair.public_key = { move(modulus), move(public_exponent) };

                    return keypair;
                } else if (first_integer == 1) {

                    dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#1 private key parse failed: Multi-prime key not supported");
                    return keypair;
                } else {
                    auto&& modulus = move(first_integer);

                    auto public_exponent_result = decoder.read<UnsignedBigInteger>();
                    if (public_exponent_result.is_error()) {

                        dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#1 public key parse failed: {}", public_exponent_result.error());
                        return keypair;
                    }

                    auto public_exponent = public_exponent_result.release_value();
                    keypair.public_key.set(move(modulus), move(public_exponent));

                    return keypair;
                }

            } else {
                if (!check_if_pkcs8_rsa_key())
                    return keypair;

                if (has_read_error)
                    return keypair;

                auto data_result = decoder.read<BitmapView>();
                if (data_result.is_error()) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#8 public key parse failed: {}", data_result.error());
                    return keypair;
                }

                auto data = data_result.release_value();

                auto padded_data_result = ByteBuffer::create_zeroed(data.size_in_bytes());
                if (padded_data_result.is_error()) {
                    dbgln_if(RSA_PARSE_DEBUG, "RSA PKCS#1 key parse failed: Not enough memory");
                    return keypair;
                }
                auto padded_data = padded_data_result.release_value();
                padded_data.overwrite(0, data.data(), data.size_in_bytes());

                return parse_rsa_key(padded_data.bytes());
            }
        }

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
