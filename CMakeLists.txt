cmake_minimum_required(VERSION 3.16)
project(pranaOS CXX ASM)

if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(FATAL_ERROR
            ": Don't use CMAKE_BUILD_TYPE when building pranaOS.\n"
            "The default build type is optimized with debug info and asserts enabled,\n"
            "and that's all there is.")
endif()

if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.2)
  message(FATAL_ERROR
      "A GCC version less than 10.2 was detected (${CMAKE_CXX_COMPILER_VERSION}), this is unsupported.\n"
      "Please re-read the build instructions documentation, and upgrade your host compiler.\n")
endif()

set(CMAKE_INSTALL_MESSAGE NEVER)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PRANAOS_ARCH "i686" CACHE STRING "Target architecture for pranaOS.")

option(ENABLE_ADDRESS_SANITIZER "Enable address sanitizer testing in gcc/clang" OFF)
option(ENABLE_KERNEL_ADDRESS_SANITIZER "Enable kernel address sanitizer testing in gcc/clang" OFF)
option(ENABLE_MEMORY_SANITIZER "Enable memory sanitizer testing in gcc/clang" OFF)
option(ENABLE_UNDEFINED_SANITIZER "Enable undefined behavior sanitizer testing in gcc/clang" OFF)
option(ENABLE_FUZZER_SANITIZER "Enable fuzzer sanitizer testing in clang" OFF)
option(ENABLE_EXTRA_KERNEL_DEBUG_SYMBOLS "Enable -Og and -ggdb3 options for Kernel code for easier debugging" OFF)
option(ENABLE_ALL_THE_DEBUG_MACROS "Enable all debug macros to validate they still compile" OFF)
option(ENABLE_ALL_DEBUG_FACILITIES "Enable all noisy debug symbols and options. Not recommended for normal developer use" OFF)
option(ENABLE_COMPILETIME_FORMAT_CHECK "Enable compiletime format string checks" ON)
option(ENABLE_PCI_IDS_DOWNLOAD "Enable download of the pci.ids database at build time" ON)
option(ENABLE_USB_IDS_DOWNLOAD "Enable download of the usb.ids database at build time" ON)
option(BUILD_LAGOM "Build parts of the system targeting the host OS for fuzzing/testing" OFF)
option(ENABLE_KERNEL_LTO "Build the kernel with link-time optimization" OFF)

include(Meta/CMake/wasm_spec_tests.cmake)

add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E env "PRANAOS_ARCH=${PRANAOS_ARCH}" ${CMAKE_SOURCE_DIR}/Meta/run.sh
    USES_TERMINAL
)
