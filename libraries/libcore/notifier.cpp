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
        set_enabled(true);
    }
} // namespace Core
