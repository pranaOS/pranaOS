/**
 * @file slavepty.h
 * @author Krisna Pranav
 * @brief slave pty
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/filesystem/inodeidentifier.h>
#include <kernel/tty/tty.h>

namespace Kernel
{
    class SlavePTY final : public TTY
    {
    private:
        /**
         * @return String 
         */
        virtual String tty_name() const override;

        /**
         * @return ssize_t 
         */
        virtual ssize_t on_tty_write(const UserOrKernelBuffer&, ssize_t) override;

        virtual void echo(u8) override;

        virtual bool can_read(const FileDescription&, size_t) const override;

        virtual KResultOr<size_t> read(FileDescription)

        RefPtr<MasterPTY> m_master;

        time_t m_time_of_last_write { 0 };

        unsigned m_index { 0 };

        char m_tty_name[32];
    }; // class SlavePTY

} // namespace Kernel