set(SOURCES
    debuginfo.cpp
    debugsession.cpp
    dwarf/abbreviationsmap.cpp
    dwarf/addressranges.cpp
    dwarf/attributevalue.cpp
    dwarf/compilationunit.cpp
    dwarf/die.cpp
    dwarf/dwarfinfo.cpp
    dwarf/expression.cpp
    dwarf/lineprogram.cpp
    processinspector.cpp
    stackframeutils.cpp
)

pranaos_lib(libdebug debug)
target_link_libraries(libdebug libc libregex)
