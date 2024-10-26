set(SOURCES
    dlfcn.cpp
)

pranaos_libc(libdl dl)
target_link_libraries(libdl libc)
