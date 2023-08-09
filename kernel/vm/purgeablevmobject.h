/**
 * @file purgeablevmobject.h
 * @author Krisna Pranav
 * @brief purgeable vm object
 * @version 1.0
 * @date 2023-08-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/vm/anonymousvmobject.h>

namespace Kernel
{
    class PurgeableVMObject final : public AnonymousVMObject 
    {
    public:
        virtual ~PurgeableVMObject() override;

        /**
         * @return NonnullRefPtr<PurgeableVMObject> 
         */
        static NonnullRefPtr<PurgeableVMObject> create_with_size(size_t);

        virtual NonnullRefPtr<VMObject> clone() override;

        int purge();

        int purge_with_interrupts_disabled(Badge<MemoryManager>);

        /**
         * @return true 
         * @return false 
         */
        bool was_purged() const 
        {
            return m_was_purged;
        }

        /**
         * @param b 
         */
        void set_was_purged(bool b)
        {
            m_was_purged = b;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_volatile() const
        {
            return m_volatile;
        }

        /**
         * @param b 
         */
        void set_volatile(bool b) 
        {
            m_volatile = b;
        }


    private:
        explicit PurgeableVMObject(size_t);
        explicit PurgeableVMObject(const PurgeableVMObject&);

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override
        {
            return "PurgeableVMObject";
        }

        int purge_impl();

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_purgeable() const override
        {
            return true;
        }

        bool m_was_purged { false };
        bool m_volatile { false };
    }; 
} // namespace Kernel
