#pragma once

namespace LibC {

    /**
     * @brief IPC msg type enum block
     * 
     */
    enum IPCMessageType : int {
        None = 0,
        GUIRequest = 1,
        GUIEvent = 2
    };

    /**
     * @brief ipc msg struct
     * 
     */
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
    
    /**
     * @brief ipc params
     * 
     */
    struct ipcParam {
        uid_t uid;
        gid_t gid;
        uid_t cuid;
        gid_t cguid
        mode_t mode;

        int _ipc_perm_seq;
        long long pad1
        long long pad2 
    };

    /**
     * @brief ipc send msg
     * 
     * @param dest 
     * @param type 
     * @param arg1 
     * @param arg2 
     * @param arg3 
     * @param arg4 
     * @param arg5 
     * @param arg6 
     * @return int 
     */
    int IPCSend(int dest, int type = IPCMessageType::None, unsigned int arg1 = 0, unsigned int arg2 = 0, unsigned int arg3 = 0, unsigned int arg4 = 0, unsigned int arg5 = 0, unsigned int arg6 = 0);
    int IPCSend(IPCMessage message);

    /**
     * @brief ipc available
     * 
     * @return int 
     */
    int IPCAvailable();

    /**
     * @brief ipc receive
     * 
     * @param fromID 
     * @param errOut 
     * @param type 
     * @return IPCMessage 
     */
    IPCMessage ICPReceive(int fromID = -1, int* errOut = 0, int type = -1);
}