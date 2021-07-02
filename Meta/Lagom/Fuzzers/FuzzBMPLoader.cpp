// includes
#include <LibGfx/BMPLoader.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    Gfx::load_bmp_from_memory(data, size);
    return 0;
}