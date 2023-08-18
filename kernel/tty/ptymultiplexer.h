/**
 * @file ptymultiplexer.h
 * @author Krisna Pranav
 * @brief pty multiplexer
 * @version 6.0
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
        MOD_MAKE_ETERNAL

    public:

        /// @brief Construct a new PTYMultiplexer object
        PTYMultiplexer();

        /// @brief Destroy the PTYMultiplexer object
        virtual ~PTYMultiplexer() override;

        /**
         * @brief initialize
         * 
         */
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
         * @return KResultOr<NonnullRefPtr<FileDescription>> 
         */
        virtual KResultOr<NonnullRefPtr<FileDescription>> open(int options) override;
        
        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override { return 0; }
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override { return 0; }

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override 
        { 
            return true; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_write(const FileDescription&, size_t) const override 
        { 
            return true; 
        }

        /**
         * @param index 
         */
        void notify_master_destroyed(Badge<MasterPTY>, unsigned index);

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
