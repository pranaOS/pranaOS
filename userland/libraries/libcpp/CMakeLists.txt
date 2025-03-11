set(SOURCES
    ast.cpp
    lexer.cpp
    parser.cpp
    preprocessor.cpp
    semanticsyntaxhighlighter.cpp
    syntaxhighlighter.cpp
    token.cpp
)

pranaos_lib(libcpp cpp)
target_link_libraries(libcpp libc libsyntax libdiff)
