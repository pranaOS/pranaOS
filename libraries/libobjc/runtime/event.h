/**
 * @file event.h
 * @author Krisna Pranav
 * @brief event
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace LibOBJC {
    class Event {
    public:
        enum Type {
            Invalid = 0,
            FdWaiterRead,
            FdWaiterWrite,
            DeferredInvoke,
            Other,
        };

        explicit Event(int type) 
            : m_type(type) {}
        
        ~Event() = default;

        int type() const {
            return m_type;
        }
        
    };
}