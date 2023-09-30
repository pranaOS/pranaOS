/**
 * @file notifier.h
 * @author Krisna Pranav
 * @brief notifier
 * @version 6.0
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <libcore/object.h>

namespace Core
{
    class Notifier : public Object
    {
    public:
        enum Event
        {
            None = 0,
            Read = 1,
            Write = 2,
            Exception = 3,
        }; // enum Event

        virtual ~Notifier() override;

        void set_enabled(bool);
        
        void close();

        int fd() const
        {
            return m_fd;
        }

        unsigned event_mask() const
        {
            return m_event_mask;
        }

    private:

        /**
         * @param fd 
         * @param event_mask 
         * @param parent 
         */
        Notifier(int fd, unsigned event_mask, Object* parent = nullptr);

        int m_fd { -1 };
        unsigned m_event_mask { 0 };
    }; // class Notifier
}