/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libcrypto/BigInt/SignedBigInteger.h>
#include <libjs/heap/Heap.h>
#include <libjs/runtime/BigInt.h>
#include <libjs/runtime/GlobalObject.h>

namespace JS {

BigInt::BigInt(Crypto::SignedBigInteger big_integer)
    : m_big_integer(move(big_integer))
{
    VERIFY(!m_big_integer.is_invalid());
}

BigInt::~BigInt()
{
}

BigInt* js_bigint(Heap& heap, Crypto::SignedBigInteger big_integer)
{
    return heap.allocate_without_global_object<BigInt>(move(big_integer));
}

BigInt* js_bigint(VM& vm, Crypto::SignedBigInteger big_integer)
{
    return js_bigint(vm.heap(), move(big_integer));
}

BigInt* number_to_bigint(GlobalObject& global_object, Value number)
{
    VERIFY(number.is_number());
    auto& vm = global_object.vm();

    if (!number.is_integral_number()) {
        vm.throw_exception<RangeError>(global_object, ErrorType::BigIntFromNonIntegral);
        return {};
    }

    return js_bigint(vm, Crypto::SignedBigInteger::create_from((i64)number.as_double()));
}

}
