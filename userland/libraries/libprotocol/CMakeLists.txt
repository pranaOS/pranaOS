set(SOURCES
    Request.cpp
    RequestClient.cpp
    WebSocket.cpp
    WebSocketClient.cpp
)

set(GENERATED_SOURCES
    ../../services/requestserver/RequestClientEndpoint.h
    ../../services/requestserver/RequestServerEndpoint.h
    ../../services/websocket/WebSocketClientEndpoint.h
    ../../services/websocket/WebSocketServerEndpoint.h
)

pranaos_lib(libprotocol protocol)
target_link_libraries(libprotocol libipc)
