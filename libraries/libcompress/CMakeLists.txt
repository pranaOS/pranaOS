set(SOURCES
    deflate.cpp
    zlib.cpp
    gzip.cpp
)

pranaos_lib(libcompress compress)
target_link_libraries(libcompress libc libcrypto)
