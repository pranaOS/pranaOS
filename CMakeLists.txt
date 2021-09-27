cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/") # add your cmake current list dir here

project(pranaOS C CXX ASM)

if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
  message(FATAL_ERROR
    ": Don't use CMAKE_BUILD_TYPE when building pranaOS.\n"
    "The default build type is optimized with debug info and asserts enabled,\n"
    "and that's all there is.")
endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL "pranaOS")
    message(FATAL_ERROR "System name is not pranaOS, this is unsupported.\n"
            "Please re-read the BuildInstructions documentation, and use the superbuild configuration\n")
endif()
