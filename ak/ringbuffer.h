#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

namespace ak {

    typedef struct ringbuffer {
        size_t size;
        size_t rBase;
        size_t dataSize;
        uint8_t* data;
    } ringbuffer_t;

    ringbuffer_t* ringbufferInit(ringbuffer_t* ref, uint8_t* buf, size_t size);
    ringbuffer_t* ringbufferNew(size_t size);
    size_t ringbufferAvaiable(ringbuffer_t* ref);
    size_t ringbufferWrite(ringbuffer_t* ref, size_t n, uint8_t* buffer);
    size_t ringbufferRead(ringbuffer_t* ref, size_t n, uint8_t* buffer);

       
}