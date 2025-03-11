/**
 * @file notifier.cpp
 * @author Krisna Pranav
 * @brief notifier
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libcore/event.h>
#include <libcore/eventloop.h>
#include <libcore/notifier.h>

namespace Core 
{
    /**
     * @brief Construct a new Notifier::Notifier object
     * 
     * @param fd 
     * @param event_mask 
     * @param parent 
     */
    Notifier::Notifier(int fd, unsigned event_mask, Object* parent)
        : Object(parent)
        , m_fd(fd)
        , m_event_mask(event_mask)
    {
        set_enabled(true);
    }

    /**
     * @brief Destroy the Notifier::Notifier object
     * 
     */
    Notifier::~Notifier()
    {
        set_enabled(false);
    }

    /**
     * @param enabled 
     */
    void Notifier::set_enabled(bool enabled)
    {
        if (m_fd < 0)
            return;

        if (enabled)
            Core::EventLoop::register_notifier({}, *this);
        else
            Core::EventLoop::unregister_notifier({}, *this);
    }

    void Notifier::close()
    {
        if (m_fd < 0)
            return;

        set_enabled(false);
        m_fd = -1;
    }

    /**
     * @param event 
     */
    void Notifier::event(Core::Event& event)
    {
        if (event.type() == Core::Event::NotifierRead && on_ready_to_read) {
            on_ready_to_read();
        } else if (event.type() == Core::Event::NotifierWrite && on_ready_to_write) {
            on_ready_to_write();
        } else {
            Object::event(event);
        }
    }

} // namespace Core
