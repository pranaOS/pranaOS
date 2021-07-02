// includes
#include <LibCompress/Deflate.h>
#include <stdio.h>

// c extern function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto result = Compress::DeflateDecompressor::decompress_all(ReadonlyBytes { data, size });
    return result.has_value();
}