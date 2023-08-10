/**
 * @file file.h
 * @author Krisna Pranav
 * @brief file
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/nonnullrefptr.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/weakable.h>
#include <kernel/forward.h>
#include <kernel/kresult.h>
#include <kernel/unixtypes.h>
#include <kernel/virtual_address.h>
#include <kernel/userorkernelbuffer.h>

namespace Kernel
{
    class File;

    class FileBlockCondition : public Thread::BlockCondition
    {
    public:
        FileBlockCondition(File& file)
            : m_file(file)
        {}

        /**
         * @param b 
         * @param data 
         * @return true 
         * @return false 
         */
        virtual bool should_add_blocker(Thread::Blocker& b, void* data) override
        {
            ASSERT(b.blocker_type() == Thread::Blocker::Type::File);

            auto& blocker = static_cast<Thread::FileBlocker&>(b);

            return !blocker.unblock(true, data);
        }

        void unblock()
        {
            ScopedSpinLock lock(m_lock);
        }

    private:
        File& m_file;
    }; // class FileBlockCondition

    class File 
        : public RefCounted<File>
        , public Weakable<File> {
        
    private:
        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void do_evaluate_block_conditions()
        {
            ASSERT(!Processor::current().in_irq());
            block_condition().unblock();
        }

        FileBlockCondition m_block_condition;

    }; // class File
} // namespace Kernel