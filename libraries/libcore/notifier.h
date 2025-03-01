/**
 * @file notifier.h
 * @author Krisna Pranav
 * @brief notifier
 * @version 6.0
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <libcore/object.h>

namespace Core 
{

    class Notifier : public Object 
    {
        C_OBJECT(Notifier)
    public:
        enum Event 
        {
            None = 0,
            Read = 1,
            Write = 2,
            Exceptional = 4,
        }; // enum Event 

        /**
         * @brief Destroy the Notifier object
         * 
         */
        virtual ~Notifier() override;

        void set_enabled(bool);

        Function<void()> on_ready_to_read;
        Function<void()> on_ready_to_write;

        void close();

        /**
         * @return int 
         */
        int fd() const 
        { 
            return m_fd; 
        }

        /**
         * @return unsigned 
         */
        unsigned event_mask() const 
        { 
            return m_event_mask; 
        }

        /**
         * @brief Set the event mask object
         * 
         * @param event_mask 
         */
        void set_event_mask(unsigned event_mask) 
        { 
            m_event_mask = event_mask; 
        }

        void event(Core::Event&) override;

    private:
        /**
         * @brief Construct a new Notifier object
         * 
         * @param fd 
         * @param event_mask 
         * @param parent 
         */
        Notifier(int fd, unsigned event_mask, Object* parent = nullptr);

        int m_fd { -1 };
        unsigned m_event_mask { 0 };
    }; // class Notifier : public Object

} // namespace Core
