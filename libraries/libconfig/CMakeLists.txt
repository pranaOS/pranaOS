set(SOURCES
    client.cpp
    listener.cpp
)

set(GENERATED_SOURCES
    ../../userland/services/configserver/configserverendpoint.h
    ../../userland/services/configserver/configclientendpoint.h
)

pranaos_lib(LibConfig config)
target_link_libraries(LibConfig LibIPC)
