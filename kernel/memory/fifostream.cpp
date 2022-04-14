#include "fifostream.h"
#include <kernel/kernel.h>
#include <kernel/log.h>

using namespace Kernel;

/**
 * @brief Construct a new FIFOStream::FIFOStream object
 * 
 * @param capacity 
 */
FIFOStream::FIFOStream(int capacity) {
    this->buffer = new char[capacity * sizeof(char)];
    this->buffer_end = (char*)this->buffer + capacity * sizeof(char);
    this->count = 0;
    this->capacity = capacity;
    this->head = this->buffer;
    this->tail = this->buffer;
}

/**
 * @brief Destroy the FIFOStream::FIFOStream object
 * 
 */
FIFOStream::~FIFOStream() {
    delete this->buffer;
}

/**
 * @brief fifostream writing object
 * 
 * @param item 
 */
void FIFOStream::write(char item) {
    if(this->count == this->capacity) {
        sendLog(Error, "Item count has reached its capacity", this->capacity);
        return;
    }
    memcpy((void*)this->head, (void*)&item, sizeof(char));
    this->head = (char*)(this->head + sizeof(char));
    if(this->head == this->buffer_end)
        this->head = this->buffer;

    this->count++;
}

/**
 * @brief fifostream read object
 * 
 * @return char 
 */
char FIFOStream::read() {
    char result = 0;
    if (this->count == 0)
        return result;
    
    memcpy((void*)&result, (void*)this->tail, sizeof(char));
    this->tail = (char*)(this->tail + sizeof(char));
    if (this->tail == this->buffer_end)
        this->tail = this->buffer;
    
    this->count--;
    return result;
}

/**
 * @brief available object
 * 
 * @return int 
 */
int FIFOStream::available() {
    return this->count;
}