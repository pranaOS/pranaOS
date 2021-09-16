#include <libio/directory.h>

namespace IO
{

bool Directory::exist()
{
    return _handle->valid();
}

}