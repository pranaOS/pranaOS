/**
 * @file cowvector.h
 * @author Krisna Pranav
 * @brief cowvector
 * @version 6.0
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2021-2025 Patrick, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/nonnullrefptr.h>
#include <mods/refcounted.h>
#include <mods/vector.h>

namespace Mods 
{

    /**
     * @tparam T 
     */
    template<typename T>
    class COWVector 
    {
        struct Detail : RefCounted<Detail> {
            Vector<T> m_members;
        }; // struct Detail : RefCounted<Detail>

    public:
        /**
         * @brief Construct a new COWVector object
         * 
         */
        COWVector()
            : m_detail(make_ref_counted<Detail>())
        {
        }

        /**
         * @brief Construct a new COWVector object
         * 
         * @param entries 
         */
        COWVector(std::initializer_list<T> entries)
            : m_detail(make_ref_counted<Detail>())
        {
            for (auto& entry : entries)
                m_detail->m_members.append(entry);
        }

        /**
         * @brief Construct a new COWVector object
         * 
         */
        COWVector(COWVector const&) = default;
        COWVector(COWVector&&) = default;

        COWVector& operator=(COWVector const&) = default;
        COWVector& operator=(COWVector&&) = default;

        /**
         * @return Vector<T> 
         */
        Vector<T> release() &&
        {
            if (m_detail->ref_count() == 1)
                return exchange(m_detail->m_members, Vector<T>());

            return m_detail->m_members;
        }

        /**
         * @param value 
         */
        void append(T const& value)
        {
            return append(T { value });
        }

        /**
         * @param value 
         */
        void append(T&& value)
        {
            copy();
            m_detail->m_members.append(move(value));
        }

        /**
         * @param values 
         */
        void extend(Vector<T>&& values)
        {
            copy();
            m_detail->m_members.extend(move(values));
        }

        /**
         * @param values 
         */
        void extend(Vector<T> const& values)
        {
            copy();
            m_detail->m_members.extend(values);
        }

        /**
         * @param values 
         */
        void extend(COWVector<T> const& values)
        {
            copy();
            m_detail->m_members.extend(values.m_detail->m_members);
        }

        /**
         * @param size 
         */
        void resize(size_t size)
        {
            copy();
            m_detail->m_members.resize(size);
        }

        /**
         * @param capacity 
         */
        void ensure_capacity(size_t capacity)
        {
            if (m_detail->m_members.capacity() >= capacity)
                return;

            copy();
            m_detail->m_members.ensure_capacity(capacity);
        }

        /**
         * @param value 
         */
        void prepend(T value)
        {
            copy();
            m_detail->m_members.prepend(move(value));
        }

        /**
         * @tparam Args 
         * @param args 
         */
        template<typename... Args>
        void empend(Args&&... args)
        {
            copy();
            m_detail->m_members.empend(forward<Args>(args)...);
        }

        void clear()
        {
            if (m_detail->ref_count() > 1)
                m_detail = make_ref_counted<Detail>();
            else
                m_detail->m_members.clear();
        }

        /**
         * @param index 
         * @return T& 
         */
        T& mutable_at(size_t index)
        {
            copy();
            return m_detail->m_members.at(index);
        }

        /**
         * @param index 
         * @return T const& 
         */
        T const& at(size_t index) const
        {
            return m_detail->m_members.at(index);
        }

        /**
         * @param index 
         * @return T const& 
         */
        T const& operator[](size_t index) const
        {
            return m_detail->m_members[index];
        }

        /**
         * @return size_t 
         */
        size_t capacity() const
        {
            return m_detail->m_members.capacity();
        }

        size_t size() const
        {
            return m_detail->m_members.size();
        }

        bool is_empty() const
        {
            return m_detail->m_members.is_empty();
        }

        /**
         * @return T const& 
         */
        T const& first() const
        {
            return m_detail->m_members.first();
        }

        /**
         * @return T const& 
         */
        T const& last() const
        {
            return m_detail->m_members.last();
        }

    private:
        void copy()
        {
            if (m_detail->ref_count() <= 1)
                return;
            auto new_detail = make_ref_counted<Detail>();
            new_detail->m_members = m_detail->m_members;
            m_detail = new_detail;
        }

        NonnullRefPtr<Detail> m_detail;

    }; // class COWVector

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::COWVector;
#endif
