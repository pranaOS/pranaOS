#include "tasksegment.h"

using namespace Kernel;
using namespace Kernel::ak;

static taskSegmentEntry tasksegment;

extern "C" void flish_tasksegment();

void taskSegment::install(int idx, int kernelSS, int kernelESP) {
    uint32_t base = (uint32_t) &tasksegment;
}

void taskSegment::setStack(int kernelSS, int kernelESP) {
    tasksegment.ss0 = kernelSS;
    tasksegment.esp0 = kernelESP;
}

taskSegmentEntry* taskSegment::getCurrent() {
    return &tasksegment;
}