// includes
#include <LibCrypto/PK/RSA.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t data, size_t size)
{
    ByteBuffer rsa_data = ByteBuffer::copy(data, size);
    Crypto::PK::RSA::parse_rsa_key(rsa_data);
    return 0;
}