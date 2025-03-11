set(SOURCES
    blockquote.cpp
    codeblock.cpp
    commentblock.cpp
    containerblock.cpp
    document.cpp
    heading.cpp
    horizontalrule.cpp
    lineiterator.cpp
    list.cpp
    paragraph.cpp
    table.cpp
    text.cpp
)

pranaos_lib(libmarkdown markdown)
target_link_libraries(libmarkdown libjs)
