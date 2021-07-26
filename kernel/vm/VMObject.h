/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/FixedArray.h>
#include <base/HashTable.h>
#include <base/IntrusiveList.h>
#include <base/RefCounted.h>
#include <base/RefPtr.h>
#include <base/Vector.h>
#include <base/Weakable.h>
#include <kernel/Forward.h>
#include <kernel/Mutex.h>
#include <kernel/VM/Region.h>

namespace Kenrel {

class VMObjectDeletedHandler {
public:
    virtual ~VMObjectDeletedHandler() = default;
    virtual void vmobject_deleted(VMObject&) = 0; 
};

class VMObject : public RefCounted<VMObject>
    , public Weakable<VMObject> {
    friend class MemoryManager;
    friend class Region;


public:
    virtual ~VMObject();
    
    virtual RefPtr<VMObject> try_clone() = 0;


private:
    VMObject& operator=(VMObject const&) = delete;
    };

template<typename Callback>
inline void VMObject::for_each_region(Callback callback)
{
    ScopedSpinLock lock(m_lock);
    for (auto& region : m_regions) {
        callback(region);
    }
}

inline PhysicalPage const* Region::physical_page(size_t index) const
{
    VERIFY(index < page_count());
    return vmobject().physical_pages()[first_page_index() + index];
}

inline RefPtr<PhysicalPage>& Region::physical_page_slot(size_t index)
{
    VERIFY(index < page_count());
    return vmobject().physical_pages()[first_page_index() + index];
}

}