cmake_minimum_required(VERSION 3.25.1)

project(libwebview 
    LANGUAGES C CXX)

option(BUILD_TESTING "Build libwebview tests" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

include(FetchContent)

FetchContent_Declare(simdjson
    GIT_REPOSITORY https://github.com/simdjson/simdjson.git
    BINARY_DIR ${CMAKE_BINARY_DIR}/thirdparty/simdjson)

FetchContent_Declare(concurrencpp
    GIT_REPOSITORY https://github.com/David-Haim/concurrencpp.git
    BINARY_DIR ${CMAKE_BINARY_DIR}/thirdparty/concurrencpp)

FetchContent_MakeAvailable(simdjson concurrencpp)

add_library(libwebview STATIC
    src/platform.cpp
    src/webview.cpp)

target_include_directories(libwebview PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(libwebview PUBLIC
    concurrencpp
    simdjson)

target_precompile_headers(libwebview PUBLIC include/precompiled.h)

if(WIN32)
    target_sources(libwebview PRIVATE src/platform/edge.cpp)

    target_include_directories(libwebview PUBLIC
        ${PROJECT_SOURCE_DIR}/thirdparty/webview2/include)

    target_link_directories(libwebview PUBLIC
        ${PROJECT_SOURCE_DIR}/thirdparty/webview2/lib/x86_64)

    target_link_libraries(libwebview PUBLIC
        dwmapi.lib
        WebView2LoaderStatic.lib
        shcore.lib
        Comctl32.lib)

    target_compile_definitions(libwebview PUBLIC LIB_WEBVIEW_EDGE)
endif()

if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk4)
    pkg_check_modules(WEBKITGTK REQUIRED webkitgtk-6.0)
    pkg_check_modules(LIBADWAITA REQUIRED libadwaita-1)

    target_sources(libwebview PRIVATE src/platform/webkit.cpp)

    target_include_directories(libwebview PUBLIC
        ${WEBKITGTK_INCLUDE_DIRS}
        ${LIBADWAITA_INCLUDE_DIRS}
        ${GTK_INCLUDE_DIRS})

    target_link_directories(libwebview PUBLIC
        ${WEBKITGTK_LIBRARY_DIRS}
        ${LIBADWAITA_LIBRARY_DIRS}
        ${GTK_LIBRARY_DIRS})

    target_link_libraries(libwebview PUBLIC
        ${WEBKITGTK_LIBRARIES}
        ${LIBADWAITA_LIBRARIES}
        ${GTK_LIBRARIES})

    target_compile_definitions(libwebview PUBLIC LIB_WEBVIEW_WEBKIT)
endif()

if(BUILD_TESTING)
    add_executable(async_test tests/async_test.cpp)
    target_link_libraries(async_test PRIVATE libwebview)
    add_executable(sync_test tests/sync_test.cpp)

    target_link_libraries(sync_test PRIVATE libwebview)
endif()