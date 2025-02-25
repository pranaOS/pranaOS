/**
 * @file listener.cpp
 * @author Krisna Pranav
 * @brief listener
 * @version 6.0
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/function.h>
#include <mods/hashtable.h>
#include <mods/string.h>
#include <libconfig/listener.h>

namespace Config 
{
    static HashTable<Listener*> s_listeners;

    /**
     * @brief Construct a new Listener::Listener object
     * 
     */
    Listener::Listener()
    {
        s_listeners.set(this);
    }

    /**
     * @brief Destroy the Listener::Listener object
     * 
     */
    Listener::~Listener()
    {
        s_listeners.remove(this);
    }   

    /**
     * @param callback 
     */
    void Listener::for_each(Function<void(Listener&)> callback)
    {
        for (auto* listener : s_listeners)
            callback(*listener);
    }
} // namespace Config 