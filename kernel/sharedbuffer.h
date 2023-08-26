/**
 * @file sharedbuffer.h
 * @author Krisna Pranav
 * @brief shared buffer
 * @version 6.0
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/ownptr.h>
#include <mods/weakptr.h>
#include <kernel/vm/memorymanager.h>
#include <kernel/vm/purgeablevmobject.h>

namespace Kernel 
{

    class SharedBuffer 
    {
    private:
        struct Reference 
        {
            Reference(ProcessID pid, unsigned count = 0)
                : pid(pid)
                , count(count)
            {
            }

            ProcessID pid;
            unsigned count { 0 };
            WeakPtr<Region> region;
        }; // struct

    public:

        /**
         * @param id 
         * @param size 
         */
        SharedBuffer(int id, int size)
            : m_shbuf_id(id)
            , m_vmobject(PurgeableVMObject::create_with_size(size))
        {
    #ifdef SHARED_BUFFER_DEBUG
            dbg() << "Created shared buffer " << m_shbuf_id << " of size " << size;
    #endif
        }

        /**
         * @brief Destroy the Shared Buffer object
         * 
         */
        ~SharedBuffer()
        {
    #ifdef SHARED_BUFFER_DEBUG
            dbg() << "Destroyed shared buffer " << m_shbuf_id << " of size " << size();
    #endif
        }

        /**
         * @param what 
         */
        void sanity_check(const char* what);

        /**
         * @param peer_pid 
         * @return true 
         * @return false 
         */
        bool is_shared_with(ProcessID peer_pid) const;

        /**
         * @param process 
         * @return void* 
         */
        void* ref_for_process_and_get_address(Process& process);

        /**
         * @param peer_pid 
         */
        void share_with(ProcessID peer_pid);

        void share_globally() 
        { 
            m_global = true; 
        }

        /**
         * @param process 
         */
        void deref_for_process(Process& process);

        /**
         * @param pid 
         * @return true 
         * @return false 
         */
        bool disown(ProcessID pid);

        /**
         * @param from_process 
         * @param with_process 
         */
        static void share_all_shared_buffers(Process& from_process, Process& with_process);

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_vmobject->size(); 
        }
        
        void destroy_if_unused();
        void seal();

        /**
         * @return PurgeableVMObject& 
         */
        PurgeableVMObject& vmobject() 
        { 
            return m_vmobject; 
        }

        /**
         * @return const PurgeableVMObject& 
         */
        const PurgeableVMObject& vmobject() const { return m_vmobject; }

        /**
         * @return int 
         */
        int id() const 
        { 
            return m_shbuf_id; 
        }

    private:
        int m_shbuf_id { -1 };
        bool m_writable { true };
        bool m_global { false };
        NonnullRefPtr<PurgeableVMObject> m_vmobject;
        Vector<Reference, 2> m_refs;
        unsigned m_total_refs { 0 };
    };

    Lockable<HashMap<int, NonnullOwnPtr<SharedBuffer>>>& shared_buffers();

} // namespace Kernel
