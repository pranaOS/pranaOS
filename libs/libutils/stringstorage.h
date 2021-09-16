#pragma once

#include <string.h>
#include <libutils/storage.h>
#include <libutils/tags.h>

namespace Utils
{

struct StringStorage final : public Storage
{
private:
    char *_buffer;
    size_t _length;

public:
    using Storage::end;
    using Storage::start;


};

}