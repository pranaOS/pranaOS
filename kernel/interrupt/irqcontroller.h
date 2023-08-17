/**
 * @file irqcontroller.h
 * @author Krisna Pranav
 * @brief irqcontroller
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/refcounted.h>
#include <mods/string.h>
#include <mods/types.h>

namespace Kernel
{
    enum class IRQControllerType
    {
        i8259 = 1,
        i82093AA = 2,
    };

    class IRQController : public RefCounted<IRQController> 
    {
    public:
        virtual ~IRQController() {};

        /// @breif: enable, disbale 
        virtual void enable(const GenericInterruptHandler&) = 0;
        virtual void disable(const GenericInterruptHandler&) = 0;

        /// @brief hard_disable
        virtual void hard_disable() 
        {
            m_hard_disabled = true;
        }

    protected:
        IRQController() {}

        virtual void initialize() = 0;

        bool m_hard_disabled { false };
    }; 
}