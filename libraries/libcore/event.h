/**
 * @file event.h
 * @author Krisna Pranav
 * @brief Event
 * @version 6.0
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/weakptr.h>
#include <libcore/deferredinvocationcontext.h>
#include <libcore/forward.h>

namespace Core 
{

    class Event 
    {
    public:
        enum Type 
        {
            Invalid = 0,
            Quit,
            Timer,
            NotifierRead,
            NotifierWrite,
            DeferredInvoke,
            ChildAdded,
            ChildRemoved,
            Custom,
        }; // enum Type 

        /**
         * @brief Construct a new Event object
         * 
         */
        Event() = default;

        /**
         * @brief Construct a new Event object
         * 
         * @param type 
         */
        explicit Event(unsigned type)
            : m_type(type)
        {
        }

        /**
         * @brief Destroy the Event object
         * 
         */
        virtual ~Event() = default;

        /**
         * @return unsigned 
         */
        unsigned type() const 
        { 
            return m_type; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_accepted() const 
        { 
            return m_accepted; 
        }
        
        void accept() 
        { 
            m_accepted = true; 
        }
        
        void ignore() 
        { 
            m_accepted = false; 
        }

    private:
        unsigned m_type { Type::Invalid };
        bool m_accepted { true };
    }; // class Event

    class DeferredInvocationEvent : public Event 
    {
        friend class EventLoop;

    public:
        /**
         * @brief Construct a new Deferred Invocation Event object
         * 
         * @param context 
         * @param invokee 
         */
        DeferredInvocationEvent(NonnullRefPtr<DeferredInvocationContext> context, Function<void()> invokee)
            : Event(Event::Type::DeferredInvoke)
            , m_context(move(context))
            , m_invokee(move(invokee))
        {
        }

    private:
        NonnullRefPtr<DeferredInvocationContext> m_context;
        Function<void()> m_invokee;
    }; // class DeferredInvocationEvent : public Event 

    class TimerEvent final : public Event 
    {
    public:
        /**
         * @brief Construct a new Timer Event object
         * 
         * @param timer_id 
         */
        explicit TimerEvent(int timer_id)
            : Event(Event::Timer)
            , m_timer_id(timer_id)
        {
        }

        /**
         * @brief Destroy the TimerEvent object
         * 
         */
        ~TimerEvent() = default;

        /**
         * @return int 
         */
        int timer_id() const 
        { 
            return m_timer_id; 
        }

    private:
        int m_timer_id;
    }; // class TimerEvent final : public Event 

    class NotifierReadEvent final : public Event 
    {
    public: 
        /**
         * @brief Construct a new NotifierReadEvent object
         * 
         * @param fd 
         */
        explicit NotifierReadEvent(int fd)
            : Event(Event::NotifierRead)
            , m_fd(fd)
        {
        }

        /**
         * @brief Destroy the NotifierReadEvent object
         * 
         */
        ~NotifierReadEvent() = default;

        /**
         * @return int 
         */
        int fd() const 
        { 
            return m_fd; 
        }

    private:
        int m_fd;
    }; // class NotifierReadEvent final : public Event 

    class NotifierWriteEvent final : public Event 
    {
    public:
        /**
         * @brief Construct a new NotifierWriteEvent object
         * 
         * @param fd 
         */
        explicit NotifierWriteEvent(int fd)
            : Event(Event::NotifierWrite)
            , m_fd(fd)
        {
        }

        /**
         * @brief Destroy the NotifierWriteEvent object
         * 
         */
        ~NotifierWriteEvent() = default;

        /**
         * @return int 
         */
        int fd() const 
        { 
            return m_fd; 
        }

    private:
        int m_fd;
    }; // class NotifierWriteEvent final : public Event 

    class ChildEvent final : public Event 
    {
    public:
        /**
         * @brief Construct a new ChildEvent object
         * 
         * @param child 
         * @param insertion_before_child 
         */
        ChildEvent(Type, Object& child, Object* insertion_before_child = nullptr);

        /**
         * @brief Destroy the ChildEvent object
         * 
         */
        ~ChildEvent() = default;

        /**
         * @return Object* 
         */
        Object* child();
        Object const* child() const;

        Object* insertion_before_child();
        Object const* insertion_before_child() const;

    private:
        WeakPtr<Object> m_child;
        WeakPtr<Object> m_insertion_before_child;
    }; // class ChildEvent final : public Event 

    class CustomEvent : public Event 
    {
    public:
        /**
         * @brief Construct a new CustomEvent object
         * 
         * @param custom_type 
         */
        CustomEvent(int custom_type)
            : Event(Event::Type::Custom)
            , m_custom_type(custom_type)
        {
        }

        /**
         * @brief Destroy the CustomEvent object
         * 
         */
        ~CustomEvent() = default;

        /**
         * @return int 
         */
        int custom_type() const 
        { 
            return m_custom_type; 
        }

    private:
        int m_custom_type { 0 };
    }; // class CustomEvent : public Event 

} // namespace Core
