set(SOURCES
    Instruction.cpp
)

pranaos_lib(libx86 x86)
target_link_libraries(libx86 libc)
