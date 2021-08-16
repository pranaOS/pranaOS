add_compile_options(-Wvla)

set(SOURCES
    asn1/ASN1.cpp
    ans1/DER.cpp
    asn1/PEM.cpp
    authentication/GHash.cpp
    bigint/Algorithms/BitwiseOperations.cpp
    bigint/Algorithms/Division.cpp
    bigint/Algorithms/GCD.cpp
    bigint/Algorithms/ModularInverse.cpp
    bigint/Algorithms/ModularPower.cpp
    bigint/Algorithms/Multiplication.cpp
    bigint/Algorithms/SimpleOperations.cpp
    bigint/SignedBigInteger.cpp
    bigint/UnsignedBigInteger.cpp
    checksum/Adler32.cpp
    checksum/CRC32.cpp
    cipher/AES.cpp
    hash/MD5.cpp
    hash/SHA1.cpp
    hash/SHA2.cpp
    numbertheory/ModularFunctions.cpp
    pk/RSA.cpp
)

pranaos_lib(libcrypto crypto)
target_link_libraries(libcrypto libc)
