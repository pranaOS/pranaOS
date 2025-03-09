add_compile_options(-Wvla)

set(SOURCES
    asn1/asn1.cpp
    asn1/der.cpp
    asn1/pem.cpp
    authentication/ghash.cpp
    authentication/poly1305.cpp
    bigint/algorithms/bitwiseoperations.cpp
    bigint/algorithms/division.cpp
    bigint/algorithms/gcd.cpp
    bigint/algorithms/modularinverse.cpp
    bigint/algorithms/modularpower.cpp
    bigint/algorithms/multiplication.cpp
    bigint/algorithms/simpleoperations.cpp
    bigint/signedbiginteger.cpp
    bigint/unsignedbiginteger.cpp
    checksum/adler32.cpp
    checksum/crc32.cpp
    cipher/aes.cpp
    cipher/chacha20.cpp
    curves/secp256r1.cpp
    curves/x25519.cpp
    curves/x448.cpp
    hash/md5.cpp
    hash/sha1.cpp
    hash/sha2.cpp
    numbertheory/modularfunctions.cpp
    pk/rsa.cpp
)

pranaos_lib(libcrypto crypto)
target_link_libraries(libcrypto libc libcore)
