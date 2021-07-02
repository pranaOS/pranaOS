// includes
#include <LibGfx/JPGLoader.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" in LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) 
{
    Gfx::load_jpg_from_memory(data, size);
    return 0;
}