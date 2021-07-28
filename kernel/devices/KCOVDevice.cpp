/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Assertions.h>
#include <base/NonnullOwnPtr.h>
#include <kernel/devices/KCOVDevice.h>
#include <kernel/devices/KCOVInstance.h>
#include <kernel/filesystem/FileDescription.h>
#include <kernel/Panic.h>
#include <libc/sys/ioctl_numbers.h>

namespace Kernel {

HashMap<ProcessID, KCOVInstance*>* KCOVDevice::proc_instance;
HashMap<ThreadID, KCOVInstance*>* KCOVDevice::thread_instance;

UNMAP_AFTER_INIT NonnullRefPtr<KCOVDevice> KCOVDevice::must_create()
{
    return adopt_ref(*new KCOVDevice);
}

UNMAP_AFTER_INIT KCOVDevice::KCOVDevice()
    : BlockDevice(30, 0)
{
    proc_instance = new HashMap<ProcessID, KCOVInstance*>();
    thread_instance = new HashMap<ThreadID, KCOVInstance*>();
    dbgln("KCOVDevice created");
}

void KCOVDevice::free_thread()
{
    auto thread = Thread::current();
    auto tid = thread->tid();

    auto maybe_kcov_instance = thread_instance->get(tid);
    if (!maybe_kcov_instance.has_value())
        return;

    auto kcov_instance = maybe_kcov_instance.value();
    VERIFY(kcov_instance->state == KCOVInstance::TRACING);
    kcov_instance->state = KCOVInstance::OPENED;
    thread_instance->remove(tid);
}

void KCOVDevice::free_process()
{
    auto process = Process::current();
    auto pid = process->pid();

    auto maybe_kcov_instance = proc_instance->get(pid);
    if (!maybe_kcov_instance.has_value())
        return;

    auto kcov_instance = maybe_kcov_instance.value();
    VERIFY(kcov_instance->state == KCOVInstance::OPENED);
    kcov_instance->state = KCOVInstance::UNUSED;
    proc_instance->remove(pid);
    delete kcov_instance;
}

KResultOr<NonnullRefPtr<FileDescription>> KCOVDevice::open(int options)
{
    auto process = Process::current();
    auto pid = process->pid();
    if (proc_instance->get(pid).has_value())
        return EBUSY; 
    auto kcov_instance = new KCOVInstance(pid);
    kcov_instance->state = KCOVInstance::OPENED;
    proc_instance->set(pid, kcov_instance);

    return File::open(options);
}

KResult KCOVDevice::ioctl(FileDescription&, unsigned request, Userspace<void*> arg)
{
    KResult return_value = KSuccess;
    auto thread = Thread::current();
    auto tid = thread->tid();
    auto pid = thread->pid();
    auto maybe_kcov_instance = proc_instance->get(pid);
    if (!maybe_kcov_instance.has_value())
        return ENXIO; 
    auto kcov_instance = maybe_kcov_instance.value();

    ScopedSpinLock lock(kcov_instance->lock);
    switch (request) {
    case KCOV_SETBUFSIZE: {
        if (kcov_instance->state >= KCOVInstance::TRACING) {
            return_value = EBUSY;
            break;
        }
        return_value = kcov_instance->buffer_allocate((FlatPtr)arg.unsafe_userspace_ptr());
        break;
    }
    case KCOV_ENABLE: {
        if (kcov_instance->state >= KCOVInstance::TRACING) {
            return_value = EBUSY;
            break;
        }
        if (!kcov_instance->has_buffer()) {
            return_value = ENOBUFS;
            break;
        }
        VERIFY(kcov_instance->state == KCOVInstance::OPENED);
        kcov_instance->state = KCOVInstance::TRACING;
        thread_instance->set(tid, kcov_instance);
        break;
    }
    case KCOV_DISABLE: {
        auto maybe_kcov_instance = thread_instance->get(tid);
        if (!maybe_kcov_instance.has_value()) {
            return_value = ENOENT;
            break;
        }
        VERIFY(kcov_instance->state == KCOVInstance::TRACING);
        kcov_instance->state = KCOVInstance::OPENED;
        thread_instance->remove(tid);
        break;
    }
    default: {
        return_value = EINVAL;
    }
    };

    return return_value;
}

KResultOr<Region*> KCOVDevice::mmap(Process& process, FileDescription&, const Range& range, u64 offset, int prot, bool shared)
{
    auto pid = process.pid();
    auto maybe_kcov_instance = proc_instance->get(pid);
    VERIFY(maybe_kcov_instance.has_value());
    auto kcov_instance = maybe_kcov_instance.value();

    if (!kcov_instance->vmobject) {
        return ENOBUFS; 
    }

    return process.space().allocate_region_with_vmobject(
        range, *kcov_instance->vmobject, offset, {}, prot, shared);
}

String KCOVDevice::device_name() const
{
    return "kcov"sv;
}

}