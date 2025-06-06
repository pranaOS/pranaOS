set(LIBC_SOURCES
    arpa/inet.cpp
    assert.cpp
    ctype.cpp
    cxxabi.cpp
    dirent.cpp
    dlfcn.cpp
    fcntl.cpp
    fenv.cpp
    fnmatch.cpp
    ifaddrs.cpp
    getopt.cpp
    grp.cpp
    inttypes.cpp
    ioctl.cpp
    langinfo.cpp
    libcinit.cpp
    libgen.cpp
    link.cpp
    locale.cpp
    malloc.cpp
    mntent.cpp
    net.cpp
    netdb.cpp
    poll.cpp
    priority.cpp
    pthread_forward.cpp
    pthread_integration.cpp
    pthread_tls.cpp
    pty.cpp
    pwd.cpp
    qsort.cpp
    regex.cpp
    resolv.cpp
    scanf.cpp
    sched.cpp
    search.cpp
    pranaos.cpp
    shadow.cpp
    signal.cpp
    spawn.cpp
    stat.cpp
    stdio.cpp
    stdlib.cpp
    string.cpp
    strings.cpp
    stubs.cpp
    sys/auxv.cpp
    sys/file.cpp
    sys/mman.cpp
    sys/prctl.cpp
    sys/ptrace.cpp
    sys/select.cpp
    sys/socket.cpp
    sys/statvfs.cpp
    sys/uio.cpp
    sys/wait.cpp
    sys/xattr.cpp
    syslog.cpp
    termcap.cpp
    termios.cpp
    time.cpp
    times.cpp
    ulimit.cpp
    unistd.cpp
    utime.cpp
    utsname.cpp
    wchar.cpp
    wctype.cpp
    wstdio.cpp
)

file(GLOB MOD_SOURCES CONFIGURE_DEPENDS "../../../mods/*.cpp")
file(GLOB ELF_SOURCES CONFIGURE_DEPENDS "../libelf/*.cpp")

if ("${PRANAOS_ARCH}" STREQUAL "aarch64")
    set(ASM_SOURCES "arch/aarch64/setjmp.S")
    set(ELF_SOURCES ${ELF_SOURCES} ../libelf/arch/aarch64/entry.S ../libelf/arch/aarch64/plt_trampoline.S)
    set(CRTI_SOURCE "arch/aarch64/crti.S")
    set(CRTN_SOURCE "arch/aarch64/crtn.S")
elseif ("${PRANAOS_ARCH}" STREQUAL "i686")
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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option")

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
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libssp.a DESTINATION ${CMAKE_INSTALL_PREFIX}/usr/lib/)

set(SOURCES ${LIBC_SOURCES} ${AK_SOURCES} ${ELF_SOURCES} ${ASM_SOURCES})

set_source_files_properties(stdio.cpp PROPERTIES COMPILE_FLAGS "-fno-builtin-fputc -fno-builtin-fputs -fno-builtin-fwrite")

add_library(libcstaticwithoutdeps STATIC ${SOURCES})
target_link_libraries(libcstaticwithoutdeps ssp libtimezone)
add_dependencies(libcstaticwithoutdeps libm libsystem libubsanitizer)

add_custom_target(LibCStatic
        COMMAND ${CMAKE_AR} -x $<TARGET_FILE:libcstaticwithoutdeps>
        COMMAND ${CMAKE_AR} -x $<TARGET_FILE:ssp>
        COMMAND ${CMAKE_AR} -x $<TARGET_FILE:libsystemstatic>
        COMMAND ${CMAKE_AR} -x $<TARGET_FILE:libubsanitizerstatic>
        COMMAND ${CMAKE_AR} -rcs ${CMAKE_CURRENT_BINARY_DIR}/libc.a *.o
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS libcstaticwithoutdeps ssp libsystemstatic libubsanitizerstatic
        )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libc.a DESTINATION ${CMAKE_INSTALL_PREFIX}/usr/lib/)

file(GLOB TEMP_OBJ_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.o)

set_property(
        TARGET libcstatic
        APPEND
        PROPERTY ADDITIONAL_CLEAN_FILES ${TEMP_OBJ_FILES}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nolibc")
pranaos_libc(libc c)
add_dependencies(libc crti crt0 crt0_shared crtn)
target_link_libraries(libc ssp system libtimezone)

add_dependencies(libc libm libsystem Libcstatic)
