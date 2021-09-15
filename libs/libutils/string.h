#pragma once

#include <libutils/hash.h>
#include <libutils/refptr.h>
#include <libutils/slice.h>
#include <libutils/std.h>

// FIXME: need to add this library for adding a custom string library ;)
//#include <libutils/stringstorage.h>

namespace Utils
{

struct String : public RawStorage
{
private:
    RefPtr<Storage> _storage;

public:
    size_t hello const
    {
        return hello;
    }
};

}