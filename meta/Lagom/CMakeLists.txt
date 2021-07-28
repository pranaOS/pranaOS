cmake_minimum_required (VERSION 3.0)
project (Lagom)


cmake_policy(SET CMP0058 NEW)

include(../CMake/wasm_spec_tests.cmake)

if (NOT ENABLE_OSS_FUZZ)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -Wno-literal-suffix -O2 -Wall -Wextra -Werror -std=c++2a -fPIC -g -Wno-deprecated-copy")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fPIC -g -Wno-deprecated-copy")
endif()

if (ENABLE_ADDRESS_SANITIZER)
    add_definitions(-fsanitize=address -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=address")
endif()

if (ENABLE_MEMORY_SANITIZER)
    add_definitions(-fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=memory -fsanitize-memory-track-origins")
endif()

if (ENABLE_UNDEFINED_SANITIZER)
    add_definitions(-fsanitize=undefined -fno-sanitize=vptr -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=undefined -fno-sanitize=vptr")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -Wno-user-defined-literals -fconstexpr-steps=16777216")

    if (ENABLE_FUZZER_SANITIZER)
        add_definitions(-fsanitize=fuzzer -fno-omit-frame-pointer)
        set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=fuzzer")
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-expansion-to-defined -Wno-literal-suffix")
endif()

set(ORIGINAL_CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(ORIGINAL_CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(ORIGINAL_CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

include_directories(../../)
include_directories(../../userland/)
include_directories(../../userland/libraries/)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (BUILD_LAGOM AND NOT ENABLE_OSS_FUZZ AND NOT ENABLE_FUZZER_SANITIZER)
    if (CMAKE_SOURCE_DIR MATCHES ".*/Lagom")
        set(write_if_different ${CMAKE_SOURCE_DIR}/../write-only-on-difference.sh)
        add_subdirectory(../../userland/libraries/libunicode/codegenerators ${CMAKE_CURRENT_BINARY_DIR}/libunicode/codegenerators)
    endif()

    include(../../userland/libraries/libunicode/unicode_data.cmake)
endif()

file(GLOB Base_SOURCES CONFIGURE_DEPENDS "../../base/*.cpp")
file(GLOB Base_TEST_SOURCES CONFIGURE_DEPENDS "../../tests/base/*.cpp")
file(GLOB LIBARCHIVE_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libarchive/*.cpp")
file(GLOB LIBAUDIO_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libaudio/*.cpp")
list(REMOVE_ITEM LIBAUDIO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../userland/libraries/libaudio/ClientConnection.cpp")
file(GLOB LIBREGEX_LIBC_SOURCES "../../userland/libraries/libregex/C/Regex.cpp")
file(GLOB LIBREGEX_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libregex/*.cpp")
file(GLOB LIBREGEX_TESTS CONFIGURE_DEPENDS "../../tests/libregex/*.cpp")
file(GLOB LIBCORE_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/LibCore/*.cpp")
file(GLOB LIBELF_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/LibELF/*.cpp")
list(REMOVE_ITEM LIBELF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../userland/libraries/LibELF/DynamicLinker.cpp")
file(GLOB LIBGEMINI_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/LibGemini/*.cpp")
file(GLOB LIBGFX_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/LibGfx/*.cpp")
file(GLOB LIBGUI_GML_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libgui/GML*.cpp")
list(REMOVE_ITEM LIBGUI_GML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../userland/libraries/libgui/GMLSyntaxHighlighter.cpp")
file(GLOB LIBHTTP_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libhttp/*.cpp")
file(GLOB LIBIPC_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libipc/*.cpp")
file(GLOB LIBLINE_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libline/*.cpp")
file(GLOB LIBMARKDOWN_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libmarkdown/*.cpp")
file(GLOB LIBX86_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libx86/*.cpp")
file(GLOB LIBJS_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libjs/*.cpp")
file(GLOB LIBJS_SUBDIR_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libjs/*/*.cpp")
file(GLOB LIBJS_SUBSUBDIR_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libjs/*/*/*.cpp")
file(GLOB LIBCOMPRESS_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/LibCompress/*.cpp")
file(GLOB LIBCOMPRESS_TESTS CONFIGURE_DEPENDS "../../tests/LibCompress/*.cpp")
file(GLOB LIBCRYPTO_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libcrypto/*.cpp")
file(GLOB LIBCRYPTO_SUBDIR_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libcrypto/*/*.cpp")
file(GLOB LIBCRYPTO_SUBSUBDIR_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libcrypto/*/*/*.cpp")
file(GLOB LIBTLS_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libtls/*.cpp")
file(GLOB LIBTTF_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libttf/*.cpp")
file(GLOB LIBTEXTCODEC_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/LibTextCodec/*.cpp")
file(GLOB SHELL_SOURCES CONFIGURE_DEPENDS "../../userland/Shell/*.cpp")
file(GLOB SHELL_TESTS CONFIGURE_DEPENDS "../../userland/Shell/tests/*.sh")
list(FILTER SHELL_SOURCES EXCLUDE REGEX ".*main.cpp$")
file(GLOB_RECURSE LIBSQL_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libsql/*.cpp")
list(REMOVE_ITEM LIBSQL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../userland/libraries/libsql/AST/SyntaxHighlighter.cpp")
list(REMOVE_ITEM LIBSQL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../userland/libraries/libsql/SQLClient.cpp")
file(GLOB LIBSQL_TEST_SOURCES CONFIGURE_DEPENDS "../../tests/libsql/*.cpp")
file(GLOB LIBWASM_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libwasm/*/*.cpp")
file(GLOB LIBIMAP_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libimap/*.cpp")
file(GLOB LIBTEST_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libtest/*.cpp")
list(FILTER LIBTEST_SOURCES EXCLUDE REGEX ".*Main.cpp$")
file(GLOB LIBTEST_MAIN CONFIGURE_DEPENDS "../../userland/libraries/libtest/TestMain.cpp")
file(GLOB LIBCRYPTO_TESTS CONFIGURE_DEPENDS "../../tests/libcrypto/*.cpp")
file(GLOB LIBTLS_TESTS CONFIGURE_DEPENDS "../../tests/libtls/*.cpp")
file(GLOB LIBUNICODE_SOURCES CONFIGURE_DEPENDS "../../userland/libraries/libunicode/*.cpp")
set(LIBUNICODE_SOURCES ${LIBUNICODE_SOURCES} ${UNICODE_DATA_SOURCES})
file(GLOB LIBUNICODE_TEST_SOURCES CONFIGURE_DEPENDS "../../tests/libunicode/*.cpp")

set(LAGOM_REGEX_SOURCES ${LIBREGEX_LIBC_SOURCES} ${LIBREGEX_SOURCES})
set(LAGOM_CORE_SOURCES ${BASE_SOURCES} ${LIBCORE_SOURCES})
set(LAGOM_MORE_SOURCES ${LIBARCHIVE_SOURCES} ${LIBAUDIO_SOURCES} ${LIBELF_SOURCES} ${LIBIPC_SOURCES} ${LIBLINE_SOURCES} ${LIBJS_SOURCES} ${LIBJS_SUBDIR_SOURCES} ${LIBJS_SUBSUBDIR_SOURCES} ${LIBX86_SOURCES} ${LIBCRYPTO_SOURCES} ${LIBCOMPRESS_SOURCES} ${LIBCRYPTO_SUBDIR_SOURCES} ${LIBCRYPTO_SUBSUBDIR_SOURCES} ${LIBTLS_SOURCES} ${LIBTTF_SOURCES} ${LIBTEXTCODEC_SOURCES} ${LIBMARKDOWN_SOURCES} ${LIBGEMINI_SOURCES} ${LIBGFX_SOURCES} ${LIBGUI_GML_SOURCES} ${LIBHTTP_SOURCES} ${LAGOM_REGEX_SOURCES} ${SHELL_SOURCES} ${LIBSQL_SOURCES} ${LIBWASM_SOURCES} ${LIBIMAP_SOURCES} ${LIBUNICODE_SOURCES})
set(LAGOM_TEST_SOURCES ${LIBTEST_SOURCES})


configure_file(../../base/Debug.h.in base/Debug.h @ONLY)
configure_file(../../Kernel/Debug.h.in Kernel/Debug.h @ONLY)

add_library(LagomCore ${LAGOM_CORE_SOURCES})
find_package(Threads REQUIRED)
target_link_libraries(LagomCore Threads::Threads)

if (BUILD_LAGOM)
    add_library(Lagom $<TARGET_OBJECTS:LagomCore> ${LAGOM_MORE_SOURCES})
    target_link_libraries(Lagom Threads::Threads)

    if (NOT ENABLE_OSS_FUZZ AND NOT ENABLE_FUZZER_SANITIZER)
        enable_testing()
        add_library(LagomTest $<TARGET_OBJECTS:LagomCore> ${LAGOM_TEST_SOURCES})
        target_link_libraries(LagomTest Threads::Threads)
        add_executable(TestApp TestApp.cpp)
        target_link_libraries(TestApp Lagom)
        target_link_libraries(TestApp stdc++)

        add_executable(TestJson TestJson.cpp)
        target_link_libraries(TestJson Lagom)
        target_link_libraries(TestJson stdc++)

        add_executable(adjtime_lagom ../../userland/utilities/adjtime.cpp)
        set_target_properties(adjtime_lagom PROPERTIES OUTPUT_NAME adjtime)
        target_link_libraries(adjtime_lagom Lagom)

        add_executable(js_lagom ../../userland/utilities/js.cpp)
        set_target_properties(js_lagom PROPERTIES OUTPUT_NAME js)
        target_link_libraries(js_lagom Lagom)
        target_link_libraries(js_lagom stdc++)
        target_link_libraries(js_lagom pthread)

        add_executable(ntpquery_lagom ../../userland/utilities/ntpquery.cpp)
        set_target_properties(ntpquery_lagom PROPERTIES OUTPUT_NAME ntpquery)
        target_link_libraries(ntpquery_lagom Lagom)

        add_executable(test-js_lagom
            ../../Tests/libjs/test-js.cpp
            ../../userland/libraries/libtest/JavaScriptTestRunnerMain.cpp)
        set_target_properties(test-js_lagom PROPERTIES OUTPUT_NAME test-js)
        target_link_libraries(test-js_lagom Lagom)
        target_link_libraries(test-js_lagom stdc++)
        target_link_libraries(test-js_lagom pthread)
        add_test(
            NAME JS
            COMMAND test-js_lagom --show-progress=false
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        add_executable(test-crypto_lagom ../../userland/Utilities/test-crypto.cpp)
        set_target_properties(test-crypto_lagom PROPERTIES OUTPUT_NAME test-crypto)
        target_link_libraries(test-crypto_lagom Lagom)
        target_link_libraries(test-crypto_lagom stdc++)
        add_test(
            NAME Crypto
            COMMAND test-crypto_lagom test -t -s google.com --ca-certs-file ../../Base/etc/ca_certs.ini
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        add_executable(test-wasm_lagom
            ../../tests/libwasm/test-wasm.cpp
            ../../userland/libraries/libtest/JavaScriptTestRunnerMain.cpp)
        set_target_properties(test-wasm_lagom PROPERTIES OUTPUT_NAME test-wasm)
        target_link_libraries(test-wasm_lagom Lagom)
        target_link_libraries(test-wasm_lagom stdc++)
        target_link_libraries(test-wasm_lagom pthread)
        add_test(
            NAME WasmParser
            COMMAND test-wasm_lagom --show-progress=false
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        set_tests_properties(WasmParser PROPERTIES
            ENVIRONMENT SERENITY_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../..
            SKIP_RETURN_CODE 1)

        add_executable(disasm_lagom ../../userland/Utilities/disasm.cpp)
        set_target_properties(disasm_lagom PROPERTIES OUTPUT_NAME disasm)
        target_link_libraries(disasm_lagom Lagom)
        target_link_libraries(disasm_lagom stdc++)

        add_executable(shell_lagom ../../userland/Shell/main.cpp)
        set_target_properties(shell_lagom PROPERTIES OUTPUT_NAME shell)
        target_link_libraries(shell_lagom Lagom)
        target_link_libraries(shell_lagom stdc++)
        target_link_libraries(shell_lagom pthread)

        add_executable(gml-format_lagom ../../userland/utilities/gml-format.cpp)
        set_target_properties(gml-format_lagom PROPERTIES OUTPUT_NAME gml-format)
        target_link_libraries(gml-format_lagom Lagom)
        target_link_libraries(gml-format_lagom stdc++)

        add_executable(test-iodevice ../../tests/libcore/TestLibCoreIODevice.cpp ${LIBTEST_MAIN})
        set_target_properties(test-iodevice PROPERTIES OUTPUT_NAME test-iodevice)
        target_link_libraries(test-iodevice Lagom)
        target_link_libraries(test-iodevice LagomTest)
        target_link_libraries(test-iodevice stdc++)
        add_test(
            NAME test-iodevice
            COMMAND test-iodevice
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../tests/libcore
        )

        add_executable(wasm_lagom ../../userland/utilities/wasm.cpp)
        set_target_properties(wasm_lagom PROPERTIES OUTPUT_NAME wasm)
        target_link_libraries(wasm_lagom Lagom)
        target_link_libraries(wasm_lagom stdc++)

        foreach(source ${LIBCRYPTO_TESTS})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBCRYPTO_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom Lagom LagomTest)
            add_test(
                    NAME ${name}_lagom
                    COMMAND ${name}_lagom
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endforeach()

        foreach(source ${LIBTLS_TESTS})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBTLS_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom Lagom LagomTest)
            add_test(
                    NAME ${name}_lagom
                    COMMAND ${name}_lagom
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../tests/libutls
            )
        endforeach()

        foreach(TEST_PATH ${SHELL_TESTS})
            get_filename_component(TEST_NAME ${TEST_PATH} NAME_WE)
            add_test(
                NAME "Shell-${TEST_NAME}"
                COMMAND shell_lagom --skip-shellrc "${TEST_PATH}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../userland/shell/tests
            )
            set_tests_properties("Shell-${TEST_NAME}" PROPERTIES
                TIMEOUT 10
                FAIL_REGULAR_EXPRESSION "FAIL"
                PASS_REGULAR_EXPRESSION "PASS"
            )
        endforeach()

        foreach(source ${AK_TEST_SOURCES})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom LagomTest)
            add_test(
                NAME ${name}_lagom
                COMMAND ${name}_lagom
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../tests/Base
            )
        endforeach()

        foreach(source ${LIBREGEX_TESTS})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LAGOM_REGEX_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom LagomTest)
            add_test(
                NAME ${name}_lagom
                COMMAND ${name}_lagom
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endforeach()

        foreach(source ${LIBCOMPRESS_TESTS})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBCOMPRESS_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom Lagom LagomTest)
            add_test(
                    NAME ${name}_lagom
                    COMMAND ${name}_lagom
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endforeach()

        foreach(source ${LIBSQL_TEST_SOURCES})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBSQL_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom LagomTest)
            add_test(
                NAME ${name}_lagom
                COMMAND ${name}_lagom
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endforeach()

        foreach(source ${LIBUNICODE_TEST_SOURCES})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBUNICODE_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom LagomTest)
            add_test(
                NAME ${name}_lagom
                COMMAND ${name}_lagom
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endforeach()
    endif()

    install(TARGETS Lagom LagomCore)
endif()

if (ENABLE_FUZZER_SANITIZER OR ENABLE_OSS_FUZZ)
    add_subdirectory(Fuzzers)
endif()