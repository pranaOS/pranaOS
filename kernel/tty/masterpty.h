/**
 * @file masterpty.h
 * @author Krisna Pranav
 * @brief masterpty
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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

        RefPtr<SlavePTY> m_slave;

        unsigned m_index;

        bool m_closed { false };

        DoubleBuffer m_buffer;

        String m_pts_name;

    }; // class MasterPTY

} // namespace Kernel