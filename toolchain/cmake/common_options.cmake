pranaos_option(ENABLE_COMPILETIME_FORMAT_CHECK ON CACHE BOOL "Enable compiletime format string checks")
pranaos_option(ENABLE_UNDEFINED_SANITIZER OFF CACHE BOOL "Enable undefined behavior sanitizer testing in gcc/clang")

pranaos_option(ENABLE_ALL_THE_DEBUG_MACROS OFF CACHE BOOL "Enable all debug macros to validate they still compile")
pranaos_option(ENABLE_ALL_DEBUG_FACILITIES OFF CACHE BOOL "Enable all noisy debug symbols and options. Not recommended for normal developer use")
pranaos_option(ENABLE_COMPILETIME_HEADER_CHECK OFF CACHE BOOL "Enable compiletime check that each library header compiles stand-alone")

pranaos_option(ENABLE_TIME_ZONE_DATABASE_DOWNLOAD ON CACHE BOOL "Enable download of the IANA Time Zone Database at build time")
pranaos_option(ENABLE_UNICODE_DATABASE_DOWNLOAD ON CACHE BOOL "Enable download of Unicode UCD and CLDR files at build time")
pranaos_option(INCLUDE_WASM_SPEC_TESTS OFF CACHE BOOL "Download and include the WebAssembly spec testsuite")

pranaos_option(HACKSTUDIO_BUILD OFF CACHE BOOL "Automatically enabled when building from HackStudio")
