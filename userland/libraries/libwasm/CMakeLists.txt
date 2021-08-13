set(SOURCES
    abstractmachine/AbstractMachine.cpp
    abstractmachine/BytecodeInterpreter.cpp
    abstractmachine/Configuration.cpp
    parser/parser.cpp
    printer/printer.cpp
)

pranaos_lib(libwasm wasm)
target_link_libraries(libwasm libc libcore)
