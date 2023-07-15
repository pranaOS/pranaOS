/**
 * @file intrusivelist.h
 * @author Krisna Pranav
 * @brief IntrusiveList
 * @version 1.0
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include "assertions.h"

namespace Mods
{
    class IntrusiveListNode;

    class IntrusiveListStorage
    {
    private:    
        friend class IntrusiveListNode;
        template<class T, IntrusiveListNode T::*member>
        friend class IntrusiveList;
        IntrusiveListNode* m_first { nullptr };
        IntrusiveListNode* m_last { nullptr };
    };  

    template<class T, IntrusiveListNode T::*member>
    class IntrusiveList
    {
        IntrusiveList();
        ~IntrusiveList();
        
        void clear();
        bool is_empty() const;
        void append(T& n);
        void prepend(T& n);
        void remove(T& n);

        bool contains(const T& ) const;

        T* first() const
        T* last() const;
        T* task_lasT();

        class Iterator
        {
        private:
            T* m_value { nullptr };
        };
    }; 
}