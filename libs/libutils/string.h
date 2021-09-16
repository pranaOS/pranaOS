#pragma once

// includes
#include <libutils/hash.h>
#include <libutils/refptr.h>
#include <libutils/slice.h>
#include <libutils/std.h>
#include <libutils/stringstorage.h>

namespace Utils
{

struct String : public RawStorage
{
private:
    RefPtr<Storage> _storage;

public:
    size_t length() const
    {
        return _storage->size();
    }

    bool empty() const
    {
        return length() == 0;
    }

    const char *cstring() const
    {
        if (!_storage)
        {
            return "";
        }
        
        return _storage->cstring();
    }

    const char &at(int index) const
    {
        return _storage->cstring()[index];
    }

    Slice slice() const
    {
        return Slice{_storage};
    }

    Slice slice(size_t start, size_t length) const
    {
        assert(start < this->length());
        assert(start + length <= this->length());

        return Slice{_storage, start, length};
    }

    String(const char *cstring = "")
    {
        _storage = make<StringStorage>(COPY, cstring);
    }

    bool operator!=(const String &other) const
    {
        return !(*this == other);
    }

    bool operator==(const String &other) const
    {
        if (_storage == other._storage)
        {
            return true;
        }

        if (length() != other.length())
        {
            return false;
        }

        for (size_t i = 0; i < length(); i++)
        {
            if (cstring()[i] != other.cstring()[i])
            {
                return false;
            }
        }
        return true;
    }

    char operator[](int index) const
    {
        return at(index);
    }

    RefPtr<Storage> storage() override
    {
        return _storage;
    }

    RefPtr<StringStorage> string_storage()
    {
        return _storage;
    }
};

template <typename T>
concept ToString = requires(const T &t) { t.string(); }

}