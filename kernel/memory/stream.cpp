#include "stream.h"
#include <system/log.h>

using namespace Kernel;

Stream::Stream() {}
Stream::~Stream() {}

char Stream::read() {
    sendLog(Error, "stream function error"
    return 0;
}

void Stream::write(char byte) {
    sendLog(Error, "stream function error");
}

int Stream::available() {
    sendLog(Error, "stream function error"
    return 0;
}