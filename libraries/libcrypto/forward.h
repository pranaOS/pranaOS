/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief forward
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>

namespace Crypto 
{

    class SignedBigInteger;
    class UnsignedBigInteger;

    /**
     * @tparam T 
     */
    template<typename T>
    concept BigInteger = IsSame<T, SignedBigInteger> || IsSame<T, UnsignedBigInteger>;

} // namespace Crypto 
