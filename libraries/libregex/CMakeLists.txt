set(SOURCES
    C/regex.cpp
    regexbytecode.cpp
    regexlexer.cpp
    regexmatcher.cpp
    regexoptimizer.cpp
    regexparser.cpp
)

pranaos_lib(libregex regex)
target_link_libraries(libregex libc libcore libunicode)
