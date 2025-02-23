set(SOURCES
    card.cpp
    cardstack.cpp
)

pranaos_lib(libcards cards)
target_link_libraries(libcards libc libcore)
