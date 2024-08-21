/**
 * @file console.h
 * @author Krisna Pranav
 * @brief console
 * @version 6.0
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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

    /** 
     * @return Console& 
     */
    static Console& the();

    static void initialize();
    static bool is_initialized();

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

    /**
     * @return Kernel::KResultOr<size_t> 
     */
    virtual Kernel::KResultOr<size_t> read(Kernel::FileDescription&, size_t, Kernel::UserOrKernelBuffer&, size_t) override;

    /**
     * @return Kernel::KResultOr<size_t> 
     */
    virtual Kernel::KResultOr<size_t> write(Kernel::FileDescription&, size_t, const Kernel::UserOrKernelBuffer&, size_t) override;
    
    /**
     * @return const char* 
     */
    virtual const char* class_name() const override 
    { 
        return "Console"; 
    }

    void put_char(char);

    /**
     * @return const CircularQueue<char, 16384>& 
     */
    const CircularQueue<char, 16384>& logbuffer() const 
    { 
        return m_logbuffer; 
    }

private:
    CircularQueue<char, 16384> m_logbuffer;
}; // class Console