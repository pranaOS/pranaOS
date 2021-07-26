/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/VM/MemoryManager.h>
#include <kernel/VM/VMObject.h>


namespace Kernel {


VMObject::VMObject(VMObject const& other)
	: m_physical_pages(other.m_physical_pages)
{
	MM.register_vmobject(*this);
}

VMObject::~VMObject()
{
	{
		ScopedSpinLock lock(m_on_deleted_lock);
		for (auto& it : m_on_deleted)
			it->vmobject_deleted(*this);
		m_on_deleted.clear();
	}

	VERIFY(m_regions.is_empty());
}

}