pranaos_install_sources("userland/libraries/libtest")

set(SOURCES
    TestSuite.cpp
    CrashTest.cpp
)

pranaos_lib(libtest test)
target_link_libraries(libtest libc)

add_library(libtestmain OBJECT TestMain.cpp)
add_library(javascripttestrunnermain OBJECT JavaScriptTestRunnerMain.cpp)
