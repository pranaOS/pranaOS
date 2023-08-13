/**
 * @file ptymultiplexer.h
 * @author Krisna Pranav
 * @brief pty multiplexer
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/badge.h>
#include <kernel/lock.h>
#include <kernel/device/characterdevice.h>

namespace Kernel
{
    class MasterPTY;


    class PTYMultiplexer final : public CharacterDevice
    {
    public:
        PTYMultiplexer();

        virtual ~PTYMultiplexer() override;

        static void initialize()
        {
            the();
        } 

        /**
         * @return PTYMultiplexer& 
         */
        static PTYMultiplexer& the();

        /**
         * @param options 
         * @return KResult<NonnullRefPtr<FileDescription>> 
         */
        virtual KResult<NonnullRefPtr<FileDescription>> open(int options) override;

        virtual bool can_read(const FileDescription&, size_t) const override
        {
            return true;
        }

        virtual bool can_write(const FileDescription&, size_t) const override
        {
            return true;
        }

    private:
        /**
         * @return const char* 
         */
        virtual const char* class_name() const override
        {
            return "PTYMultiplexer";
        }

        Lock m_lock { "PTYMultiplexer" };

        Vector<unsigned> m_freelist;


    }; // class PTYMultiplexer 
    
} // namespace Kernel