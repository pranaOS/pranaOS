/**
 * @file localserver.h
 * @author Krisna Pranav
 * @brief local server
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Core
{
    
    class LocalServer : public Object
    {
    public:
        /**
         * @brief Destroy the LocalServer object
         * 
         */
        virtual ~LocalServer() override;

    private:
        /**
         * @brief Construct a new LocalServer object
         * 
         * @param parent 
         */
        explicit LocalServer(Object* parent = nullptr);

        void setup_notifier();

        int m_fd { -1 };
        bool m_listening { false };
        RefPtr<Notifier> m_notifier;
    }; // class LocalServer : public Object

} // namespace Core
