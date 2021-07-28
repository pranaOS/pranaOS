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
#include <libc/sys/ioctl_numbers.h>
#include <kernel/Panic.h>

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