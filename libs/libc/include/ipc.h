#pragma once

namespace Ipc {

    enum IPCMessageType : int
    {
        None = 0,
        GUIRequest = 1,
        GUIEvent = 2
    };
    
    struct IPCMessage {
        int source;
        int dest;
        int type;
    }
}