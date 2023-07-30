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

namespace Mods {

    template<typename PtrType, int inline_capacity = 0>
    class NonnullPtrVector : public Vector<PtrType, inline_capacity> {
        using T = typename PtrType::ElementType;
        using Base = Vector<PtrType, inline_capacity>;

    public:
        NonnullPtrVector()
        {}

        /**
         * @param other 
         */
        NonnullPtrVector(Vector<PtrType>&& other) : Base(static_cast<Base&&>(other))
        {}

        /**
         * @brief Construct a new Nonnull Ptr Vector object
         * 
         * @param other 
         */
        NonnullPtrVector(const Vector<PtrType>& other) : Base(static_cast<const Base&>(other))
        {}

        using Base::size;

        using ConstIterator = SimpleIterator<const NonnullPtrVector, const T>;
        using Iterator = SimpleIterator<NonnullPtrVector, T>;

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr ConstIterator begin() const 
        { 
            return ConstIterator::begin(*this); 
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr Iterator begin() 
        { 
            return Iterator::begin(*this); 
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr ConstIterator end() const 
        { 
            return ConstIterator::end(*this); 
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr Iterator end() 
        { 
            return Iterator::end(*this); 
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE PtrType& ptr_at(int index) 
        { 
            return Base::at(index); 
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const PtrType& ptr_at(int index) const 
        { 
            return Base::at(index); 
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& at(int index) 
        { 
            return *Base::at(index);
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& at(int index) const 
        { 
            return *Base::at(index); 
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& operator[](int index) 
        { 
            return at(index); 
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& operator[](int index) const 
        { 
            return at(index); 
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& first() 
        { 
            return at(0); 
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& first() const 
        { 
            return at(0); 
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& last() 
        { 
            return at(size() - 1); 
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& last() const 
        { 
            return at(size() - 1); 
        }

    private:
        /**
         * @return *void
         * 
         */
        void resize(int) = delete;
    };

}
