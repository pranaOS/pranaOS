/**
 * @file nonnull_ptrvector.h
 * @author Krisna Pranav
 * @brief NonNullPtrVector
 * @version 1.0
 * @date 2023-07-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "vector.h"

namespace Mods
{
    template<typename PtrType, int inline_capacity = 0>
    class NonnullPtrVector: public Vector<PtrType, inline_capacity>
    {
        using T = typename PtrType::ElementType;
        using Base = Vector<PtrType, inline_capacity>;

    public:
        NonnullPtrVector()
        {}

        NonnullPtrVector(Vector<PtrType> && other) : Base(static_cast<Base&&>(other))
        {}

        NonnullPtrVector(const Vector<PtrType>& other) : Base(static_cast<const Base&>(other))
        {}

        using Base::size;

        using ConstIterator = SimpleIterator<const NonnullPtrVector, const T>;
        using Iterator = SimpleIterator<NonnullRefPtr, T>;

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr ConstIterator begin() const
        {
            return ConstIterator::begin(*this);
        }

    private:
        /**
         * @return *void
         * 
         */
        void resize(int) = delete;
    };
} // namespace Mods

