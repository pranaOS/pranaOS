/**
 * @file secp256r1.cpp
 * @author Krisna Pranav
 * @brief secp256r1
 * @version 6.0
 * @date 2025-03-08
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/bytereader.h>
#include <mods/endian.h>
#include <mods/random.h>
#include <mods/string.h>
#include <mods/stringbuilder.h>
#include <mods/ufixedbigint.h>
#include <libcrypto/curves/secp256r1.h>

namespace Crypto::Curves 
{

    static constexpr u256 REDUCE_PRIME { u128 { 0x0000000000000001ull, 0xffffffff00000000ull }, u128 { 0xffffffffffffffffull, 0x00000000fffffffe } };
    static constexpr u256 REDUCE_ORDER { u128 { 0x0c46353d039cdaafull, 0x4319055258e8617bull }, u128 { 0x0000000000000000ull, 0x00000000ffffffff } };
    static constexpr u256 PRIME_INVERSE_MOD_R { u128 { 0x0000000000000001ull, 0x0000000100000000ull }, u128 { 0x0000000000000000ull, 0xffffffff00000002ull } };
    static constexpr u256 PRIME { u128 { 0xffffffffffffffffull, 0x00000000ffffffffull }, u128 { 0x0000000000000000ull, 0xffffffff00000001ull } };
    static constexpr u256 R2_MOD_PRIME { u128 { 0x0000000000000003ull, 0xfffffffbffffffffull }, u128 { 0xfffffffffffffffeull, 0x00000004fffffffdull } };
    static constexpr u256 ONE { 1u };
    static constexpr u256 B_MONTGOMERY { u128 { 0xd89cdf6229c4bddfull, 0xacf005cd78843090ull }, u128 { 0xe5a220abf7212ed6ull, 0xdc30061d04874834ull } };

    /**
     * @param data 
     * @return u256 
     */
    static u256 import_big_endian(ReadonlyBytes data)
    {
        VERIFY(data.size() == 32);

        u64 d = Mods::convert_between_host_and_big_endian(ByteReader::load64(data.offset_pointer(0 * sizeof(u64))));
        u64 c = Mods::convert_between_host_and_big_endian(ByteReader::load64(data.offset_pointer(1 * sizeof(u64))));
        u64 b = Mods::convert_between_host_and_big_endian(ByteReader::load64(data.offset_pointer(2 * sizeof(u64))));
        u64 a = Mods::convert_between_host_and_big_endian(ByteReader::load64(data.offset_pointer(3 * sizeof(u64))));

        return u256 { u128 { a, b }, u128 { c, d } };
    }

    /**
     * @param value 
     * @param data 
     */
    static void export_big_endian(u256 const& value, Bytes data)
    {
        u64 a = Mods::convert_between_host_and_big_endian(value.low().low());
        u64 b = Mods::convert_between_host_and_big_endian(value.low().high());
        u64 c = Mods::convert_between_host_and_big_endian(value.high().low());
        u64 d = Mods::convert_between_host_and_big_endian(value.high().high());

        ByteReader::store(data.offset_pointer(0 * sizeof(u64)), d);
        ByteReader::store(data.offset_pointer(1 * sizeof(u64)), c);
        ByteReader::store(data.offset_pointer(2 * sizeof(u64)), b);
        ByteReader::store(data.offset_pointer(3 * sizeof(u64)), a);
    }

    /**
     * @param left 
     * @param right 
     * @param condition 
     * @return u256 
     */
    static u256 select(u256 const& left, u256 const& right, bool condition)
    {
        u256 mask = (u256)condition - 1;

        return (left & mask) | (right & ~mask);
    }

    /**
     * @param left 
     * @param right 
     * @return u512 
     */
    static u512 multiply(u256 const& left, u256 const& right)
    {
        auto result = left.wide_multiply(right);
        return { result.low, result.high };
    }

    /**
     * @param value 
     * @return u256 
     */
    static u256 modular_reduce(u256 const& value)
    {
        bool carry = false;
        u256 other = value.addc(REDUCE_PRIME, carry);

        return select(value, other, carry);
    }

    /**
     * @param value 
     * @return u256 
     */
    static u256 modular_reduce_order(u256 const& value)
    {
        bool carry = false;
        u256 other = value.addc(REDUCE_ORDER, carry);

        return select(value, other, carry);
    }

    /**
     * @param left 
     * @param right 
     * @param carry_in 
     * @return u256 
     */
    static u256 modular_add(u256 const& left, u256 const& right, bool carry_in = false)
    {
        bool carry = carry_in;
        u256 output = left.addc(right, carry);

        u64 t = carry;
        carry = false;
        u256 addend { u128 { t, -(t << 32) }, u128 { -t, (t << 32) - (t << 1) } };
        output = output.addc(addend, carry);

        t = carry;
        addend = { u128 { t, -(t << 32) }, u128 { -t, (t << 32) - (t << 1) } };
        return output + addend;
    }

    /**
     * @param left 
     * @param right 
     * @return u256 
     */
    static u256 modular_sub(u256 const& left, u256 const& right)
    {
        bool borrow = false;
        u256 output = left.subc(right, borrow);

        u64 t = borrow;
        borrow = false;
        u256 sub { u128 { t, -(t << 32) }, u128 { -t, (t << 32) - (t << 1) } };
        output = output.subc(sub, borrow);

        t = borrow;
        sub = { u128 { t, -(t << 32) }, u128 { -t, (t << 32) - (t << 1) } };
        return output - sub;
    }

    /**
     * @param left 
     * @param right 
     * @return u256 
     */
    static u256 modular_multiply(u256 const& left, u256 const& right)
    {
        u512 mult = multiply(left, right);

        u512 m = multiply(mult.low(), PRIME_INVERSE_MOD_R);

        u512 mp = multiply(m.low(), PRIME);

        bool carry = false;
        mult.low().addc(mp.low(), carry);

        return modular_add(mult.high(), mp.high(), carry);
    }

    /**
     * @param value 
     * @return u256 
     */
    static u256 modular_square(u256 const& value)
    {
        return modular_multiply(value, value);
    }

    /**
     * @param value 
     * @return u256 
     */
    static u256 to_montgomery(u256 const& value)
    {
        return modular_multiply(value, R2_MOD_PRIME);
    }

    /**
     * @param value 
     * @return u256 
     */
    static u256 from_montgomery(u256 const& value)
    {
        return modular_multiply(value, ONE);
    }

    /**
     * @param value 
     * @return u256 
     */
    static u256 modular_inverse(u256 const& value)
    {
        u256 base = value;

        u256 result = value;
        base = modular_square(base);

        base = modular_square(base);

        for (auto i = 0; i < 94; i++) {
            result = modular_multiply(result, base);
            base = modular_square(base);
        }

        for (auto i = 0; i < 96; i++) {
            base = modular_square(base);
        }

        result = modular_multiply(result, base);
        base = modular_square(base);

        for (auto i = 0; i < 31; i++) {
            base = modular_square(base);
        }

        for (auto i = 0; i < 32; i++) {
            result = modular_multiply(result, base);
            base = modular_square(base);
        }

        return result;
    }

    /**
     * @param output_point 
     * @param point 
     */
    static void point_double(JacobianPoint& output_point, JacobianPoint const& point)
    {
        if (point.y.is_zero_constant_time()) {
            VERIFY_NOT_REACHED();
        }

        u256 temp;

        u256 y2 = modular_square(point.y);

        u256 s = modular_multiply(point.x, y2);
        s = modular_add(s, s);
        s = modular_add(s, s);

        temp = modular_square(point.z);
        u256 m = modular_add(point.x, temp);
        temp = modular_sub(point.x, temp);
        m = modular_multiply(m, temp);
        temp = modular_add(m, m);
        m = modular_add(m, temp);

        u256 xp = modular_square(m);
        xp = modular_sub(xp, s);
        xp = modular_sub(xp, s);

        u256 yp = modular_sub(s, xp);
        yp = modular_multiply(yp, m);
        temp = modular_square(y2);
        temp = modular_add(temp, temp);
        temp = modular_add(temp, temp);
        temp = modular_add(temp, temp);
        yp = modular_sub(yp, temp);

        u256 zp = modular_multiply(point.y, point.z);
        zp = modular_add(zp, zp);

        output_point.x = xp;
        output_point.y = yp;
        output_point.z = zp;
    }

    /**
     * @param output_point 
     * @param point_a 
     * @param point_b 
     */
    static void point_add(JacobianPoint& output_point, JacobianPoint const& point_a, JacobianPoint const& point_b)
    {
        if (point_a.x.is_zero_constant_time() && point_a.y.is_zero_constant_time() && point_a.z.is_zero_constant_time()) {
            output_point.x = point_b.x;
            output_point.y = point_b.y;
            output_point.z = point_b.z;
            return;
        }

        u256 temp;

        temp = modular_square(point_b.z);

        u256 u1 = modular_multiply(point_a.x, temp);

        u256 s1 = modular_multiply(point_a.y, temp);
        s1 = modular_multiply(s1, point_b.z);

        temp = modular_square(point_a.z);

        u256 u2 = modular_multiply(point_b.x, temp);

        u256 s2 = modular_multiply(point_b.y, temp);
        s2 = modular_multiply(s2, point_a.z);

        if (u1.is_equal_to_constant_time(u2)) {
            if (s1.is_equal_to_constant_time(s2)) {
                point_double(output_point, point_a);
                return;
            } else {
                VERIFY_NOT_REACHED();
            }
        }

        u256 h = modular_sub(u2, u1);
        u256 h2 = modular_square(h);
        u256 h3 = modular_multiply(h2, h);

        u256 r = modular_sub(s2, s1);

        u256 x3 = modular_square(r);
        x3 = modular_sub(x3, h3);
        temp = modular_multiply(u1, h2);
        temp = modular_add(temp, temp);
        x3 = modular_sub(x3, temp);

        u256 y3 = modular_multiply(u1, h2);
        y3 = modular_sub(y3, x3);
        y3 = modular_multiply(y3, r);
        temp = modular_multiply(s1, h3);
        y3 = modular_sub(y3, temp);

        u256 z3 = modular_multiply(h, point_a.z);
        z3 = modular_multiply(z3, point_b.z);

        output_point.x = x3;
        output_point.y = y3;
        output_point.z = z3;
    }

    /**
     * @param point 
     */
    static void convert_jacobian_to_affine(JacobianPoint& point)
    {
        u256 temp;

        temp = modular_square(point.z);
        temp = modular_inverse(temp);
        point.x = modular_multiply(point.x, temp);

        temp = modular_square(point.z);
        temp = modular_multiply(temp, point.z);
        temp = modular_inverse(temp);
        point.y = modular_multiply(point.y, temp);
    }

    /**
     * @param point 
     * @return true 
     * @return false 
     */
    static bool is_point_on_curve(JacobianPoint const& point)
    {
        u256 temp, temp2;

        temp = modular_square(point.y);
        temp2 = modular_square(point.x);
        temp2 = modular_multiply(temp2, point.x);
        temp = modular_sub(temp, temp2);
        temp = modular_add(temp, point.x);
        temp = modular_add(temp, point.x);
        temp = modular_add(temp, point.x);
        temp = modular_sub(temp, B_MONTGOMERY);
        temp = modular_reduce(temp);

        return temp.is_zero_constant_time();
    }

    /**
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> SECP256r1::generate_private_key()
    {
        auto buffer = TRY(ByteBuffer::create_uninitialized(32));
        fill_with_random(buffer.data(), buffer.size());
        return buffer;
    }

    /**
     * @param a 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> SECP256r1::generate_public_key(ReadonlyBytes a)
    {
        u8 generator_bytes[65] {
            0x04,
            0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47, 0xF8, 0xBC, 0xE6, 0xE5, 0x63, 0xA4, 0x40, 0xF2,
            0x77, 0x03, 0x7D, 0x81, 0x2D, 0xEB, 0x33, 0xA0, 0xF4, 0xA1, 0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96,
            0x4F, 0xE3, 0x42, 0xE2, 0xFE, 0x1A, 0x7F, 0x9B, 0x8E, 0xE7, 0xEB, 0x4A, 0x7C, 0x0F, 0x9E, 0x16,
            0x2B, 0xCE, 0x33, 0x57, 0x6B, 0x31, 0x5E, 0xCE, 0xCB, 0xB6, 0x40, 0x68, 0x37, 0xBF, 0x51, 0xF5,
        };

        return compute_coordinate(a, { generator_bytes, 65 });
    }

    /**
     * @param scalar_bytes 
     * @param point_bytes 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> SECP256r1::compute_coordinate(ReadonlyBytes scalar_bytes, ReadonlyBytes point_bytes)
    {
        VERIFY(scalar_bytes.size() == 32);

        u256 scalar = import_big_endian(scalar_bytes);

        scalar = modular_reduce_order(scalar);
        if (scalar.is_zero_constant_time())
            return Error::from_string_literal("SECP256r1: scalar is zero");


        if (point_bytes.size() != 65 || point_bytes[0] != 0x04)
            return Error::from_string_literal("SECP256r1: point is not uncompressed format");

        JacobianPoint point {
            import_big_endian(point_bytes.slice(1, 32)),
            import_big_endian(point_bytes.slice(33, 32)),
            1u,
        };

        point.x = to_montgomery(point.x);
        point.y = to_montgomery(point.y);
        point.z = to_montgomery(point.z);

        if (!is_point_on_curve(point))
            return Error::from_string_literal("SECP256r1: point is not on the curve");

        JacobianPoint result;
        JacobianPoint temp_result;

        for (auto i = 0; i < 256; i++) {
            point_add(temp_result, result, point);

            auto condition = (scalar & 1u) == 1u;
            result.x = select(result.x, temp_result.x, condition);
            result.y = select(result.y, temp_result.y, condition);
            result.z = select(result.z, temp_result.z, condition);

            point_double(point, point);
            scalar >>= 1u;
        }

        convert_jacobian_to_affine(result);

        VERIFY(is_point_on_curve(result));

        result.x = from_montgomery(result.x);
        result.y = from_montgomery(result.y);

        result.x = modular_reduce(result.x);
        result.y = modular_reduce(result.y);

        auto buf = TRY(ByteBuffer::create_uninitialized(65));
        buf[0] = 0x04;
        export_big_endian(result.x, buf.bytes().slice(1, 32));
        export_big_endian(result.y, buf.bytes().slice(33, 32));
        return buf;
    }

    /**
     * @param shared_point 
     * @return ErrorOr<ByteBuffer> 
     */
    ErrorOr<ByteBuffer> SECP256r1::derive_premaster_key(ReadonlyBytes shared_point)
    {
        VERIFY(shared_point.size() == 65);
        VERIFY(shared_point[0] == 0x04);

        ByteBuffer premaster_key = TRY(ByteBuffer::create_uninitialized(32));
        premaster_key.overwrite(0, shared_point.data() + 1, 32);
        return premaster_key;
    }

} // namespace Crypto::Curves
