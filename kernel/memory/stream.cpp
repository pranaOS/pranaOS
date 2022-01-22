#include "stream.h"
#include <system/log.h>

using namespace Kernel;

Stream::Stream() {}
Stream::~Stream() {}

char Stream::read() {
    Log(Error, "stream function called");
    return 0;
}

void Stream::write(char byte) {
    Log(Error, "stream function called");
}

int Stream::available()
{
    Log(Error, "stream function called");
    return 0;
}