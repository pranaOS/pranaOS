#include <ipc.h>
#include <string.h>

using namespace IPC;

int IPC::IPCSend(int dest, int type, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5, unsigned int arg6) {
    IPCMessage message;

    message.dest = dest;
    message.type = type;
    message.source = Process::ID;

    message.arg1 = arg1;
    message.arg2 = arg2;
    message.arg3 = arg3;
    message.arg4 = arg4;
    message.arg5 = arg5;
    message.arg6 = arg6;

    return IPCSend(message);
}