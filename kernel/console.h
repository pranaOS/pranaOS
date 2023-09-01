/**
 * @file console.h
 * @author Krisna Pranav
 * @brief console
 * @version 6.0
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/circular_queue.h>
#include <mods/vector.h>
#include <kernel/device/characterdevice.h>

class Console final : public Kernel::CharacterDevice
{
    MOD_MAKE_ETERNAL

public:
    static Console& the();

    Console();
    virtual ~Console() override;

    /**
     * @return true 
     * @return false 
     */
    virtual bool can_read(const Kernel::FileDescription&, size_t) const override;
    
    virtual bool can_write(const Kernel::FileDescription&, size_t) const override
    {
        return true;
    }

private:
    CircularQueue<char, 16384> m_logbuffer;
}; // class Console.