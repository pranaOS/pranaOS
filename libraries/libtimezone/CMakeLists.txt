include(${pranaOS_SOURCE_DIR}/Meta/CMake/time_zone_data.cmake)

set(SOURCES
    timezone.cpp
    ${TIME_ZONE_DATA_SOURCES}
)

add_library(libtimezone OBJECT ${SOURCES})
target_compile_definitions(libtimezone PRIVATE ENABLE_TIME_ZONE_DATA=$<BOOL:${ENABLE_TIME_ZONE_DATABASE_DOWNLOAD}>)
