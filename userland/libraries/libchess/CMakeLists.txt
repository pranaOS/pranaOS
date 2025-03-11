set(SOURCES
    chess.cpp
    ucicommand.cpp
    uciendpoint.cpp
)

pranaos_lib(libchess chess)
target_link_libraries(libchess libc libcore)
