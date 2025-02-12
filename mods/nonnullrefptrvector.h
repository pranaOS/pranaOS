/**
 * @file nonnullrefptrvector.h
 * @author Krisna Pranav
 * @brief nonnullrefptrvector
 * @version 6.0
 * @date 2023-07-30
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/nonnullptrvector.h>
#include <mods/nonnullrefptr.h>

namespace Mods
{
    /**
     * @tparam T
     * @tparam inline_capacity
     */
    template <typename T, size_t inline_capacity>
    class NonnullRefPtrVector : public NonnullPtrVector<NonnullRefPtr<T>, inline_capacity>
    {
        using NonnullPtrVector<NonnullRefPtr<T>, inline_capacity>::NonnullPtrVector;
    };
} // namespace Mods

using Mods::NonnullRefPtrVector;
