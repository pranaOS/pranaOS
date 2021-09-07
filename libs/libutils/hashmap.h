/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/hash.h>
#include <libutils/vector.h>

namespace Utils
{

template <typename TKey, typename TValue>
struct HashMap
{
private:
        struct Item
    {
        uint32_t hash;
        TKey key;
        TValue value;
    };

    static constexpr int BUCKET_COUNT = 256;

    Vector<Vector<Item>> _buckets{};

    Vector<Item> &bucket(uint32_t hash)
    {
        return _buckets[hash % BUCKET_COUNT];
    }

    Item *item_by_key(const TKey &key)
    {
        return item_by_key(key, hash<TKey>(key));
    }

        Item *item_by_key(const TKey &key, uint32_t hash)
    {
        Item *result = nullptr;
        auto &b = bucket(hash);

        b.foreach([&](Item &item) {
            if (item.hash == hash && item.key == key)
            {
                result = &item;
                return Iteration::STOP;
            }
            else
            {
                return Iteration::CONTINUE;
            }
        });

        return result;
    }
};

}