#include "fifostream.h"
#include <kernel/kernel.h>

using namespace Kernel;

/**
 * @brief Construct a new FIFOStream::FIFOStream object
 * 
 * @param capacity 
 */
FIFOStream::FIFOStream(int capacity) {
    this->buffer = new char[capacity * sizeof(char)];
}

/**
 * @brief Destroy the FIFOStream::FIFOStream object
 * 
 */
FIFOStream::~FIFOStream() {
    delete this->buffer;
}
