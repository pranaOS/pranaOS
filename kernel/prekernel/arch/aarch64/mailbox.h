/**
 * @file mailbox.h
 * @author Krisna Pranav
 * @brief Mail Box
 * @version 6.0
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Prekernel 
{
    class Mailbox 
    {
    public:
        class Message 
        {
        protected:
            /**
             * @brief Construct a new Message object
             * 
             * @param tag 
             * @param arguments_size 
             */
            Message(u32 tag, u32 arguments_size);

        private:
            u32 m_tag;
            u32 m_arguments_size;
            u32 m_command_tag;
        }; // class Message

        class MessageHeader 
        {
        public:
            /**
             * @brief Construct a new Message Header object
             * 
             */
            MessageHeader();

            /**
             * @return u32 
             */
            u32 queue_size() 
            { 
                return m_message_queue_size; 
            }

            void set_queue_size(u32 size) 
            { 
                m_message_queue_size = size; 
            }

            bool success() const;

        private:
            u32 m_message_queue_size;
            u32 m_command_tag;
        }; // class MessageHeader

        class MessageTail 
        {
        private:
            u32 m_empty_tag = 0;
        }; // class MessageTail

        static Mailbox& the();

        /**
         * @param queue 
         * @param queue_size 
         * @return true 
         * @return false 
         */
        bool send_queue(void* queue, u32 queue_size) const;
    }; // class Mailbox
} // namespace Kernel