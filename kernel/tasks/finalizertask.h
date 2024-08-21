/**
 * @file finalizertask.h
 * @author Krisna Pranav
 * @brief Finalizer Task
 * @version 6.0
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Kernel 
{
    class FinalizerTask 
    {
    public:
        /// @brief spawn task.
        static void spawn();
    };
}
