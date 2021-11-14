cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

project(pranaOS LANGUAGES C CXX ASM)

if(${pranaOS_SOURCE_DIR} STREQUAL ${pranaOS_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${pranaOS_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(${pranaOS_SOURCE_DIR}/cmake/arch_detector.cmake)

include(${pranaOS_SOURCE_DIR}/cmake/platform_detector.cmake)

include(${pranaOS_SOURCE_DIR}/cmake/header_files.cmake)

include(${pranaOS_SOURCE_DIR}/cmake/find_asm_files.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(KernelName kernel.elf)

set(pranaOS_SOURCE_CODE_DIR ${pranaOS_SOURCE_DIR}/src)
add_subdirectory(${pranaOS_SOURCE_CODE_DIR})