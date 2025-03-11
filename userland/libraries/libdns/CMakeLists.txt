set(SOURCES
    answer.cpp
    name.cpp
    packet.cpp
)

pranaos_lib(libdns dns)
target_link_libraries(libdns libc)
