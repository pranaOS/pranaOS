set(SOURCES
    AST.cpp
    Lexer.cpp
    Parser.cpp
    Preprocessor.cpp
    SyntaxHighlighter.cpp
    Token.cpp
)

pranaos_lib(libcpp cpp)
target_link_libraries(libcpp libc libyntax)
