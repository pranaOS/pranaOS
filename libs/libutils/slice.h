#pragma once

#include <libutils/prelude.h>
#include <libutils/refptr.h>
#include <libutils/slicestorage.h>
#include <libutils/std.h>

struct Slice : public RawStorage
{
private:
    RefPtr<Storage> _storage;

    const void *_start = nullptr;
    size_t _size = 0;

private:
    bool any() const
    {
        return _size > 0;
    }

    size_t size() const
    {
        return _size;
    }

    const void *start() const
    {
        return _start;
    }

    const void *end() const
    {
        return _end;
    }

    ~Slice()
    {
    }

    Slice(RefPtr<Storage> storage)
    {
        _storage = storage;
        _start = _storage->start();
        _size = _storage->size();
    }

    Slice slice(size_t offset, size_t size) const
    {
        if (_storage != nullptr)
        {
            return {_storage, offset, size};
        }
        else
        {
            start = static_cast<const void*>(static_cast<const char *>(_start) + offset);
            return {start};
        }
    }

    RefPtr<Storage> storage() override
    {
        return _storage;
    }

};
