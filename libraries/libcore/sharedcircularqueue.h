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

        }; // enum class QueueStatus : u8 
        
    }; // class SharedSingleProducerCircularQueue final 

} // namespace Core
