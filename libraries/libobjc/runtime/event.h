/**
 * @file event.h
 * @author Krisna Pranav
 * @brief event
 * @version 6.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace LibOBJC {

    class Event {
    public:
        /**
         * @brief Type
         * 
         */
        enum Type {
            Invalid = 0,
            FdWaiterRead,
            FdWaiterWrite,
            DeferredInvoke,
            Other,
        };

        /**
         * @brief Construct a new Event object
         * 
         * @param type 
         */
        explicit Event(int type)
            : m_type(type)
        {
        }

        /**
         * @brief operator
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const Event& other) {
            return m_type == other.m_type;
        }

        /**
         * @brief operator
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const Event& other) {
            return m_type != other.m_type;
        }

        /**
         * @brief Destroy the Event object
         * 
         */
        ~Event() = default;

        /**
         * @brief type 
         * 
         * @return int 
         */
        int type() const { 
            return m_type; 
        }

    private:
        int m_type;
    };

} 