/**
 * @file nonnull_ptrvector.h
 * @author Krisna Pranav
 * @brief NonNullPtrVector
 * @version 6.0
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/vector.h>

namespace Mods
{
    /**
     * @tparam PtrType 
     * @tparam inline_capacity 
     */
    template <typename PtrType, size_t inline_capacity = 0>
    class NonnullPtrVector : public Vector<PtrType, inline_capacity>
    {
        using T = typename PtrType::ElementType;
        using Base = Vector<PtrType, inline_capacity>;

    public:
        /**
         * @brief Construct a new Nonnull Ptr Vector object
         * 
         */
        NonnullPtrVector() = default;   

        /**
         * @brief Construct a new Nonnull Ptr Vector object
         * 
         * @param other 
         */
        NonnullPtrVector(Vector<PtrType>&& other)
            : Base(static_cast<Base&&>(other))
        {
        }

        /**
         * @brief Construct a new Nonnull Ptr Vector object
         * 
         * @param other 
         */
        NonnullPtrVector(Vector<PtrType> const& other)
            : Base(static_cast<Base const&>(other))
        {
        }

        using Base::size;

        using ConstIterator = SimpleIterator<const NonnullPtrVector, const T>;
        using Iterator = SimpleIterator<NonnullPtrVector, T>;
        using ReverseIterator = SimpleReverseIterator<NonnullPtrVector, T>;
        using ReverseConstIterator = SimpleReverseIterator<NonnullPtrVector const, T const>;

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
        ALWAYS_INLINE constexpr ReverseIterator rbegin()
        {
            return ReverseIterator::rbegin(*this);
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr ReverseConstIterator rbegin() const
        {
            return ReverseConstIterator::rbegin(*this);
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
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr ReverseIterator rend()
        {
            return ReverseIterator::rend(*this);
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr ReverseConstIterator rend() const
        {
            return ReverseConstIterator::rend(*this);
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr auto in_reverse()
        {
            return ReverseWrapper::in_reverse(*this);
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr auto in_reverse() const
        {
            return ReverseWrapper::in_reverse(*this);
        }

        /**
         * @param value 
         * @return Optional<size_t> 
         */
        Optional<size_t> find_first_index(T const& value) const
        {
            if(auto const index = Mods::find_index(begin(), end(), value);
            index < size())
            {
                return index;
            }
            return {};
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE PtrType& ptr_at(size_t index)
        {
            return Base::at(index);
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE PtrType const& ptr_at(size_t index) const
        {
            return Base::at(index);
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& at(size_t index)
        {
            return *Base::at(index);
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& at(size_t index) const
        {
            return *Base::at(index);
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& operator[](size_t index)
        {
            return at(index);
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& operator[](size_t index) const
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
        void resize(size_t) = delete;
    }; // class NonnullPtrVector : public Vector<PtrType, inline_capacity>
} // namespace Mods
