add_subdirectory(base)
if (${PRANAOS_ARCH} STREQUAL "i686" AND NOT USE_CLANG_TOOLCHAIN)
    add_subdirectory(userspaceemulator)
endif()
