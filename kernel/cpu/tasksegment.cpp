#include "tasksegment.h"

using namespace Kernel;
using namespace Kernel::core;

static taskSegmentEntry taskSegment;

extern "C" void flush_tasksegment();

void taskSegment::install(uint32_t idx, uint32_t kernelSS, uint32_t kernelESP) {
	memOperator::memset(&taskSegment, 0, sizeof(taskSegmentEntry));

	uint32_t base = (uint32_t) &taskSegment;
	globalDescriptorTable::setDescriptor (idx, base, base + sizeof (taskSegmentEntry), 0xE9, 0);

    memOperator::memset ((void*) &taskSegment, 0, sizeof (taskSegmentEntry));

	taskSegment.ss0 = kernelSS;
	taskSegment.esp0 = kernelESP;
	taskSegment.iomap = sizeof(taskSegmentEntry);

	flush_tasksegment();
}

void taskSegment::setStack(uint32_t kernelSS, uint32_t kernelESP) {
    taskSegment.ss0 = kernelSS;
    taskSegment.esp0 = kernelESP;
}

taskSegmentEntry* taskSegment::getCurrent() {
	return &taskSegment;
}