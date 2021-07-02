// includes
#include <LibELF/Image.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    ELF::Image elf(data, size, /* */false);
    return 0;
}