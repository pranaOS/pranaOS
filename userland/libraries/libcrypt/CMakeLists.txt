set(SOURCES
    crypt.cpp
)

pranaos_libc(libcrypt crypt)
target_link_libraries(libcrypt libc libcrypto)
