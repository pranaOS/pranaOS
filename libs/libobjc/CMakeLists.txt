set(LIBOBJC_SOURCES
    "include/libfouncation/NSObject.h",
    "include/libobjc/class.h",
    "include/libobjc/helpers.h",
    "include/libobjc/isa.h",
    "include/libobjc/memory.h",
    "include/libobjc/objc.h",
    "include/libobjc/runtime.h",
    "include/libobjc/selector.h",
    "include/libobjc/v1/decls.h",

    "src/class.m",
    "src/init.m",
)

file(GLOB LIOBJC_SOURCES CONFIGURE_DEPENDS "include/")
