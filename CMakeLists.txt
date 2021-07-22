cmake_minimum_required(VERSION 3.16)
project(pranaOS CXX ASM)

if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(FATAL_ERROR
            ": Don't use CMAKE_BUILD_TYPE when building pranaOS.\n"
            "The default build type is optimized with debug info and asserts enabled,\n"
            "and that's all there is.")
endif()
