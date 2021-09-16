#pragma once

#include <libio/seek.h>

namespace IO
{

struct Writer
{
    virtual ~Writer() {}

    virtual JResult flush()
    {
        return SUCCESS;
    }
};

template <typename T>
concept SeekableWriter = IsBaseOf<Writer, T>::value &&IsBaseOf<Seek, T>::value;

}