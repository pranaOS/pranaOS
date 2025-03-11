set(SOURCES
    format.cpp
    generator.cpp
    hunks.cpp
)

pranaos_lib(libdiff diff)
target_link_libraries(libdiff libc)
