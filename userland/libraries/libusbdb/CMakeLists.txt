set(SOURCES
    database.cpp
)

pranaos_lib(libusbdb usbdb)
target_link_libraries(libusbdb libc)
