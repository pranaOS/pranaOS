set(SOURCES
    Line.cpp
    Terminal.cpp
    TerminalWidget.cpp
    EscapeSequenceParser.cpp
)

set(GENERATED_SOURCES
    EscapeSequenceStateMachine.h
)

generate_state_machine(StateMachine.txt EscapeSequenceStateMachine.h)
pranaos_lib(libvt vt)
target_link_libraries(libvt libc libcore libgui libgfx libdesktop)
