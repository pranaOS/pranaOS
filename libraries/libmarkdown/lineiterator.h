/**
 * @file lineiterator.h
 * @author Krisna Pranav
 * @brief Line Iterator
 * @version 6.0
 * @date 2024-10-31
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/iterator.h>
#include <mods/string_view.h>
#include <mods/vector.h>

namespace Markdown 
{
    /**
     * @tparam T 
     */
    template<typename T>
    class FakePtr 
    {
    public:
        /**
         * @brief Construct a new Fake Ptr object
         * 
         * @param item 
         */
        FakePtr(T item)
            : m_item(move(item))
        {
        }

        /**
         * @return T const* 
         */
        T const* operator->() const 
        { 
            return &m_item; 
        }

        /**
         * @return T* 
         */
        T* operator->() 
        { 
            return &m_item; 
        }

    private:
        T m_item;
    }; // class FakePtr

    class LineIterator 
    {
    public:
        struct Context 
        {
            enum class Type 
            {
                ListItem,
                BlockQuote,
            }; // enum class Type 

            Type type;
            size_t indent;
            bool ignore_prefix;

            /**
             * @param indent 
             * @return Context 
             */
            static Context list_item(size_t indent) 
            { 
                return { Type::ListItem, indent, true }; 
            }

            /**
             * @return Context 
             */
            static Context block_quote() 
            { 
                return { Type::BlockQuote, 0, false }; 
            }
        }; // struct Context 

        /**
         * @brief Construct a new Line Iterator object
         * 
         * @param lines 
         */
        LineIterator(Vector<StringView>::ConstIterator const& lines)
            : m_iterator(lines)
        {
        }

        bool is_end() const;
        StringView operator*() const;

        /**
         * @return LineIterator 
         */
        LineIterator operator++()
        {
            reset_ignore_prefix();
            ++m_iterator;
            return *this;
        }

        /**
         * @return LineIterator 
         */
        LineIterator operator++(int)
        {
            LineIterator tmp = *this;
            reset_ignore_prefix();
            ++m_iterator;
            return tmp;
        }

        /**
         * @param delta 
         * @return LineIterator 
         */
        LineIterator operator+(ptrdiff_t delta) const
        {
            LineIterator copy = *this;
            copy.reset_ignore_prefix();
            copy.m_iterator = copy.m_iterator + delta;
            return copy;
        }
        
        /**
         * @param delta 
         * @return LineIterator 
         */
        LineIterator operator-(ptrdiff_t delta) const
        {
            LineIterator copy = *this;
            copy.reset_ignore_prefix();
            copy.m_iterator = copy.m_iterator - delta;
            return copy;
        }

        /**
         * @param other 
         * @return ptrdiff_t 
         */
        ptrdiff_t operator-(LineIterator other) const 
        { 
            return m_iterator - other.m_iterator; 
        }

        /**
         * @return FakePtr<StringView> 
         */
        FakePtr<StringView> operator->() const 
        { 
            return FakePtr<StringView>(operator*()); 
        }

        /**
         * @param context 
         */
        void push_context(Context context) 
        { 
            m_context_stack.append(move(context)); 
        }

        void pop_context() 
        { 
            m_context_stack.take_last(); 
        }

    private:
        void reset_ignore_prefix();

        /**
         * @param line 
         * @return Optional<StringView> 
         */
        Optional<StringView> match_context(StringView line) const;

        Vector<StringView>::ConstIterator m_iterator;
        Vector<Context> m_context_stack;
    }; // class LineIterator
} // namespace Markdown
