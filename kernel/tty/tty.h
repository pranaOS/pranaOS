/**
 * @file tty.h
 * @author Krisna Pranav
 * @brief tty
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/doublebuffer.h>
#include <kernel/processgroup.h>
#include <kernel/unixtypes.h>
#include <mods/circular_deque.h>
#include <mods/weakptr.h>
#include <kernel/device/characterdevice.h>

namespace Kernel
{
    class TTY : public CharacterDevice
    {
    public: 
        /// @brief Destroy the TTY object
        virtual ~TTY() override;

        /**
         * @return KResult<size_t> 
         */
        virtual KResult<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override;

        /**
         * @return KResult<size_t> 
         */
        virtual KResult<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;
        virtual bool can_write(const FileDescription&, size_t) const override;

        /**
         * @return String 
         */
        virtual String tty_name() const = 0;

        unsigned short rows() const
        {
            return m_rows;
        }

        unsigned short columns() const
        {
            return m_columns;
        }

        /**
         * @return ProcessGroupID 
         */
        ProcessGroupID pgid() const
        {
            if (auto pg = m_pg.strong_ref())
                return pg->pgid();
            
            return 0;           
        }

    private:
        virtual bool is_tty() const final override
        {
            return true;
        }

        CircularDequeu<u8, 1024> m_input_buffer;

        WeakPtr<Process> m_original_process_parent;
        WeakPtr<ProcessGroup> m_pg;

        termios m_termios;

        unsigned short m_rows { 0 };
        unsigned short m_columns { 0 };
    }; // class TTY
    
} // namespace Kernel