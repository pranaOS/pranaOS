// includes
#include <LibCompress/Deflate.h>
#include <stdio.h>

// extern function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto result = Compress::DeflateCompressor::compress_all(ReadonlyBytes { data, size });
    return result.has_value();
}