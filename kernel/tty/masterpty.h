/**
 * @file masterpty.h
 * @author Krisna Pranav
 * @brief masterpty
 * @version 6.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/badge.h>
#include <kernel/doublebuffer.h>
#include <kernel/device/characterdevice.h>

namespace Kernel 
{

    class SlavePTY;

    class MasterPTY final : public CharacterDevice 
    {
    public:

        /**
         * @param index 
         */
        explicit MasterPTY(unsigned index);

        /// @brief Destroy the MasterPTY object
        virtual ~MasterPTY() override;

        /**
         * @return unsigned 
         */
        unsigned index() const 
        { 
            return m_index; 
        }

        /**
         * @return String 
         */
        String pts_name() const;

        /**
         * @return ssize_t 
         */
        ssize_t on_slave_write(const UserOrKernelBuffer&, ssize_t);

        /**
         * @return true 
         * @return false 
         */
        bool can_write_from_slave() const;

        /// @brief: notify_slave_closed
        void notify_slave_closed(Badge<SlavePTY>);

        /**
         * @return true 
         * @return false 
         */
        bool is_closed() const 
        { 
            return m_closed; 
        }

        /**
         * @return String 
         */
        virtual String absolute_path(const FileDescription&) const override;

    private:
        
        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override;
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;
        virtual bool can_write(const FileDescription&, size_t) const override;

        /**
         * @return KResult 
         */
        virtual KResult close() override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_master_pty() const override 
        { 
            return true; 
        }

        /**
         * @param request 
         * @param arg 
         * @return int 
         */
        virtual int ioctl(FileDescription&, unsigned request, FlatPtr arg) override;

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "MasterPTY"; 
        }

        RefPtr<SlavePTY> m_slave;

        unsigned m_index;

        bool m_closed { false };

        DoubleBuffer m_buffer;
        
        String m_pts_name;

    }; // class MasterPTY

} // namespace Kernel