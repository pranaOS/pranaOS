set(SOURCES
    decoder.cpp
)

pranaos_lib(libtextcodec textcodec)
target_link_libraries(libtextcodec libc)
