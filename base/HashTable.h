/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Forward.h>
#include <base/HashFunctions.h>
#include <base/StdLibExtras.h>
#include <base/Types.h>
#include <base/kmalloc.h>


namespace Base {

enum class HashSetResult {
    InsertedNewEntry,
    ReplacedExistingEntry,
    KeptExistingEntry
};

enum class HashSetExistingEntryBehavior {
    Keep,
    Replace
};

template<typename HashTableType, typename T, typename BucketType>
class HashTableIterator {
    friend HashTableType;

public:
    bool operator==(const HashTableIterator& other) const { return m_bucket == other.m_bucket; }
    bool operator!=(const HashTableIterator& other) const { return m_bucket != other.m_bucket; }
    T& operator*() { return *m_bucket->slot(); }
    T* operator->() { return m_bucket->slot(); }
    void operator++() { skip_to_next(); }

};

}
