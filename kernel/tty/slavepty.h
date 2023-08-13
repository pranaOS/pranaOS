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

    class MasterPTY;

    class SlavePTY final : public TTY 
    {
    public:

        /// @brief Destroy the SlavePTY object
        virtual ~SlavePTY() override;

        void on_master_write(const UserOrKernelBuffer&, ssize_t);

        /**
         * @return unsigned 
         */
        unsigned index() const 
        { 
            return m_index; 
        }

        /**
         * @return time_t 
         */
        time_t time_of_last_write() const 
        { 
            return m_time_of_last_write; 
        }

        /**
         * @return FileBlockCondition& 
         */
        virtual FileBlockCondition& block_condition() override;

    private:
        
        /**
         * @return String 
         */
        virtual String tty_name() const override;

        /**
         * @return ssize_t 
         */
        virtual ssize_t on_tty_write(const UserOrKernelBuffer&, ssize_t) override;

        /// @brief: echo
        virtual void echo(u8) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_write(const FileDescription&, size_t) const override;

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "SlavePTY"; 
        }

        /**
         * @return KResult 
         */
        virtual KResult close() override;

        friend class MasterPTY;

        SlavePTY(MasterPTY&, unsigned index);

        RefPtr<MasterPTY> m_master;

        time_t m_time_of_last_write { 0 };

        unsigned m_index { 0 };
        
        char m_tty_name[32];

    }; // class SlavePTY 

} // namespace Kernel
