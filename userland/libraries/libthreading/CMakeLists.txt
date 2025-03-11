set(SOURCES
    backgroundaction.cpp
    thread.cpp
)

pranaos_lib(libthreading threading)
target_link_libraries(libthreading libc libcore libpthread)
