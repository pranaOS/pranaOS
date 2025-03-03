/**
 * @file sharedcircularqueue.h
 * @author Krisna Pranav
 * @brief shared circular queue
 * @version 6.0
 * @date 2025-03-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

namespace Core
{

    template<typename T, size_t Size = 32>

    requires(popcout(Size) == 1)
    class SharedSingleProducerCircularQueue final 
    {
    public:
        using ValueType = T;

        enum class QueueStatus : u8 
        {
            INVALID,
            FULL,
            EMPTY,
        }; // enum class QueueStatus : u8 

        SharedSingleProducerCircularQueue() = default;

        constexpr size_t size() const
        {
            return Size;
        }

    private:
        struct SharedMemorySPCQ
        {

        }; // struct SharedMemorySPCQ

        String m_name {};
    }; // class SharedSingleProducerCircularQueue final 

} // namespace Core
