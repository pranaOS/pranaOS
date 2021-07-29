set(LIBC_SOURCES
    assert.cpp
    ctype.cpp
)

file(GLOB BASE_SOURCES CONFIGURE_DEPENDS "../../../base/*.cpp")
file(GLOB ELF_SOURCES CONFIGURE_DEPENDS "../libelf/*.cpp")

if ("${PRANAOS_ARCH}" STREQUAL "i686")
    set(ASM_SOURCES "arch/i386/setjmp.S")
    set(ELF_SOURCES ${ELF_SOURCES} ../libelf/arch/i386/entry.S ../libelf/arch/i386/plt_trampoline.S)
    set(CRTI_SOURCE "arch/i386/crti.S")
    set(CRTN_SOURCE "arch/i386/crtn.S")
elseif ("${PRANAOS_ARCH}" STREQUAL "x86_64")
    set(ASM_SOURCES "arch/x86_64/setjmp.S")
    set(ELF_SOURCES ${ELF_SOURCES} ../libelf/arch/x86_64/entry.S ../libelf/arch/x86_64/plt_trampoline.S)
    set(CRTI_SOURCE "arch/x86_64/crti.S")
    set(CRTN_SOURCE "arch/x86_64/crtn.S")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -DPRANAOS_LIBC_BUILD")

add_library(crt0 STATIC crt0.cpp)
add_custom_command(
    TARGET crt0
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crt0> ${CMAKE_INSTALL_PREFIX}/usr/lib/crt0.o
)
add_library(crt0_shared STATIC crt0_shared.cpp)
add_custom_command(
    TARGET crt0_shared
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crt0_shared> ${CMAKE_INSTALL_PREFIX}/usr/lib/crt0_shared.o
)

add_library(crti STATIC ${CRTI_SOURCE})
add_custom_command(
    TARGET crti
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crti> ${CMAKE_INSTALL_PREFIX}/usr/lib/crti.o
)

add_library(crtn STATIC ${CRTN_SOURCE})
add_custom_command(
    TARGET crtn
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crtn> ${CMAKE_INSTALL_PREFIX}/usr/lib/crtn.o
)

set_source_files_properties (ssp.cpp PROPERTIES COMPILE_FLAGS
    "-fno-stack-protector")
add_library(ssp STATIC ssp.cpp)
add_custom_command(
    TARGET ssp
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:ssp> ${CMAKE_INSTALL_PREFIX}/usr/lib/ssp.o
)

set(SOURCES ${LIBC_SOURCES} ${AK_SOURCES} ${ELF_SOURCES} ${ASM_SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
add_library(libcStaticWithoutDeps STATIC ${SOURCES})
target_link_libraries(libcStaticWithoutDeps ssp)
add_dependencies(libcStaticWithoutDeps LibM LibSystem LibUBSanitizer)

add_custom_target(libcStatic
        COMMAND ${CMAKE_AR} -x $<TARGET_FILE:libcStaticWithoutDeps>
        COMMAND ${CMAKE_AR} -x $<TARGET_FILE:ssp>
        COMMAND ${CMAKE_AR} -x $<TARGET_FILE:LibSystemStatic>
        COMMAND ${CMAKE_AR} -x $<TARGET_FILE:LibUBSanitizerStatic>
        COMMAND ${CMAKE_AR} -qcs ${CMAKE_CURRENT_BINARY_DIR}/libc.a *.o
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS libcStaticWithoutDeps ssp LibSystemStatic LibUBSanitizerStatic
        )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libc.a DESTINATION ${CMAKE_INSTALL_PREFIX}/usr/lib/)
file(GLOB TEMP_OBJ_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.o)
set_property(
        TARGET libcStatic
        APPEND
        PROPERTY ADDITIONAL_CLEAN_FILES ${TEMP_OBJ_FILES}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
pranaos_libc(libc c)
add_dependencies(libc crti crt0 crt0_shared crtn)
target_link_libraries(libc ssp system)

add_dependencies(libc LibM LibSystem libcStatic)