set(MODS_SOURCES
    assertions.cpp
    base64.cpp
    circularbuffer.cpp
    constrainedstream.cpp
    countingstream.cpp
    dospackedtime.cpp
    deprecatedflystring.cpp
    byteString.cpp
    error.cpp
    floatingpointstringconversions.cpp
    flyString.cpp
    format.cpp
    fuzzymatch.cpp
    genericlexer.cpp
    hex.cpp
    jsonobject.cpp
    jsonparser.cpp
    jsonpath.cpp
    jsonvalue.cpp
    lexicalPath.cpp
    memorystream.cpp
    numberformat.cpp
    optionparser.cpp
    random.cpp
    siphash.cpp
    slugify.cpp
    stackinfo.cpp
    stream.cpp
    string.cpp
    stringbase.cpp
    stringbuilder.cpp
    stringfloatingpointconversions.cpp
    stringimpl.cpp
    stringutils.cpp
    stringview.cpp
    time.cpp
    uuid.cpp
    utf16view.cpp
    utf32view.cpp
    utf8view.cpp
    kmalloc.cpp
)

list(TRANSFORM MODS_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(MODS_SOURCES ${MODS_SOURCES} PARENT_SCOPE)

pranaos_install_headers(Mods)
pranaos_install_sources(Mods)
