/**
 * @file sharedbuffer.cpp
 * @author Krisna Pranav
 * @brief shared buffer
 * @version 6.0
 * @date 2023-09-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/singleton.h>
#include <kernel/process.h>
#include <kernel/sharedbuffer.h>

namespace Kernel 
{

    static Mods::Singleton<Lockable<HashMap<int, NonnullOwnPtr<SharedBuffer>>>> s_map;

    /**
     * @return Lockable<HashMap<int, NonnullOwnPtr<SharedBuffer>>>& 
     */
    Lockable<HashMap<int, NonnullOwnPtr<SharedBuffer>>>& shared_buffers()
    {
        return *s_map;
    }

    /**
     * @param what 
     */
    void SharedBuffer::sanity_check(const char* what)
    {
        LOCKER(shared_buffers().lock(), Lock::Mode::Shared);

        unsigned found_refs = 0;

        for (const auto& ref : m_refs)
            found_refs += ref.count;

        if (found_refs != m_total_refs) {
            dbg() << what << " SharedBuffer{" << this << "} id: " << m_shbuf_id << " has total refs " << m_total_refs << " but founded " << found_refs;
            for (const auto& ref : m_refs) {
                dbg() << "    ref from pid " << ref.pid.value() << ": refcnt " << ref.count;
            }
            ASSERT_NOT_REACHED();
        }
    }

    /**
     * @param peer_pid 
     * @return true 
     * @return false 
     */
    bool SharedBuffer::is_shared_with(ProcessID peer_pid) const
    {
        LOCKER(shared_buffers().lock(), Lock::Mode::Shared);

        if (m_global)
            return true;
        for (auto& ref : m_refs) {
            if (ref.pid == peer_pid) {
                return true;
            }
        }

        return false;
    }

    /**
     * @param process 
     * @return void* 
     */
    void* SharedBuffer::ref_for_process_and_get_address(Process& process)
    {
        LOCKER(shared_buffers().lock());
        ASSERT(is_shared_with(process.pid()));

        if (m_global) {
            bool found = false;
            for (auto& ref : m_refs) {
                if (ref.pid == process.pid()) {
                    found = true;
                    break;
                }
            }
            if (!found)
                m_refs.append(Reference(process.pid()));
        }

        for (auto& ref : m_refs) {
            if (ref.pid == process.pid()) {
                if (!ref.region) {
                    auto* region = process.allocate_region_with_vmobject(VirtualAddress(), size(), m_vmobject, 0, "SharedBuffer", PROT_READ | (m_writable ? PROT_WRITE : 0));
                    if (!region)
                        return (void*)-ENOMEM;
                    ref.region = region;
                    region->set_shared(true);
                }
                ref.count++;
                m_total_refs++;
                sanity_check("ref_for_process_and_get_address");
                return ref.region.unsafe_ptr()->vaddr().as_ptr(); 
            }
        }
        ASSERT_NOT_REACHED();
    }

    /**
     * @param peer_pid 
     */
    void SharedBuffer::share_with(ProcessID peer_pid)
    {
        LOCKER(shared_buffers().lock());

        if (m_global)
            return;
        for (auto& ref : m_refs) {
            if (ref.pid == peer_pid) {
                sanity_check("share_with (old ref)");
                return;
            }
        }

        m_refs.append(Reference(peer_pid));
        sanity_check("share_with (new ref)");
    }

    /**
     * @param from_process 
     * @param with_process 
     */
    void SharedBuffer::share_all_shared_buffers(Process& from_process, Process& with_process)
    {
        LOCKER(shared_buffers().lock());

        for (auto& shbuf : shared_buffers().resource()) {
            auto& shared_buffer = *shbuf.value;
            for (auto& ref : shared_buffer.m_refs) {
                if (ref.pid == from_process.pid()) {
                    auto ref_count = ref.count;
                    shared_buffer.m_refs.append(Reference(with_process.pid(), ref_count));
                    shared_buffer.m_total_refs += ref_count;
                    break;
                }
            }
        }
    }

    /**
     * @param process 
     */
    void SharedBuffer::deref_for_process(Process& process)
    {
        LOCKER(shared_buffers().lock());

        for (size_t i = 0; i < m_refs.size(); ++i) {
            auto& ref = m_refs[i];
            if (ref.pid == process.pid()) {
                ASSERT(ref.count > 0);
                ref.count--;
                ASSERT(m_total_refs > 0);
                m_total_refs--;
                if (ref.count == 0) {
    #ifdef SHARED_BUFFER_DEBUG
                    dbg() << "Releasing shared buffer reference on " << m_shbuf_id << " of size " << size() << " by PID " << process.pid().value();
    #endif
                    process.deallocate_region(*ref.region.unsafe_ptr()); 
    #ifdef SHARED_BUFFER_DEBUG
                    dbg() << "Released shared buffer reference on " << m_shbuf_id << " of size " << size() << " by PID " << process.pid().value();
    #endif
                    sanity_check("deref_for_process");
                    destroy_if_unused();
                    return;
                }
                return;
            }
        }

        ASSERT_NOT_REACHED();
    }

    /**
     * @param pid 
     * @return true 
     * @return false 
     */
    bool SharedBuffer::disown(ProcessID pid)
    {
        LOCKER(shared_buffers().lock());

        for (size_t i = 0; i < m_refs.size(); ++i) {
            auto& ref = m_refs[i];
            if (ref.pid == pid) {
    #ifdef SHARED_BUFFER_DEBUG
                dbg() << "Disowning shared buffer " << m_shbuf_id << " of size " << size() << " by PID " << pid.value();
    #endif
                ASSERT(m_total_refs >= ref.count);
                m_total_refs -= ref.count;
                m_refs.unstable_take(i);
    #ifdef SHARED_BUFFER_DEBUG
                dbg() << "Disowned shared buffer " << m_shbuf_id << " of size " << size() << " by PID " << pid.value();
    #endif
                destroy_if_unused();
                break;
            }
        }

        return m_total_refs == 0;
    }

    void SharedBuffer::destroy_if_unused()
    {
        LOCKER(shared_buffers().lock());
        sanity_check("destroy_if_unused");
        if (m_total_refs == 0) {
    #ifdef SHARED_BUFFER_DEBUG
            dbg() << "Destroying unused SharedBuffer{" << this << "} id: " << m_shbuf_id;
    #endif
            auto count_before = shared_buffers().resource().size();

            shared_buffers().resource().remove(m_shbuf_id);
            ASSERT(count_before != shared_buffers().resource().size());
        }
    }

    void SharedBuffer::seal()
    {
        LOCKER(shared_buffers().lock());
        m_writable = false;
        for (auto& ref : m_refs) {
            if (auto* region = ref.region.unsafe_ptr()) {
                region->set_writable(false);
                region->remap();
            }
        }
    }
} // namespace Kernel