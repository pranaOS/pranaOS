/**
 * @file device.h
 * @author Krisna Pranav
 * @brief device
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <mods/doublelinkedlist.h>
#include <mods/hashmap.h>
#include <kernel/lock.h>
#include <kernel/unixtypes.h>
#include <kernel/device/asyncdevicerequest.h>
#include <kernel/filesystem/file.h>

namespace Kernel
{
    class Device : public File 
    {

    protected:
        /**
         * @param major 
         * @param minor 
         */
        Device(unsigned major, unsigned minor);

        /**
         * @param uid 
         */
        void set_uid(uid_t uid)
        {
            m_uid = uid;
        }

        /**
         * @param gid 
         */
        void set_gid(gid_t gid)
        {
            m_gid = gid;
        }
    
    private:
        unsigned m_major { 0 };
        unsigned m_minor { 0 };

        uid_t m_uid { 0 };
        gid_t m_gid { 0 };

        SpinLock<u8> m_requests_lock;

        DoubleLinkedList<RefPtr<AsyncDeviceRequest>> m_request;

    }; // class Device

} // namespace Kernel