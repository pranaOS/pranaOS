include(${pranaOS_SOURCE_DIR}/Meta/CMake/pnp_ids.cmake)

set(SOURCES
    dmt.cpp
    edid.cpp
    vic.cpp
    ${PNP_IDS_SOURCES}
)

pranaos_lib(libedid edid)
target_link_libraries(libedid libc)
target_compile_definitions(libedid PRIVATE ENABLE_PNP_IDS_DATA=$<BOOL:${ENABLE_PNP_IDS_DOWNLOAD}>)
