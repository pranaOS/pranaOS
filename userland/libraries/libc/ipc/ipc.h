#pragma once

namespace LibC {

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

        unsigned int arg1;
        unsigned int arg2;
        unsigned int arg3;
        unsigned int arg4;
        unsigned int arg5;
        unsigned int arg6;
    };

    int IPCSend(int dest, int type = IPCMessageType::None, unsigned int arg1 = 0, unsigned int arg2 = 0, unsigned int arg3 = 0, unsigned int arg4 = 0, unsigned int arg5 = 0, unsigned int arg6 = 0);
    int IPCSend(IPCMessage message);
    int IPCAvailable();

    IPCMessage ICPReceive(int fromID = -1, int* errOut = 0, int type = -1);
}