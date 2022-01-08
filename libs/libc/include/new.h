#pragma once

#include <stddef.h>
#include <heap.h>

using namespace pranaOSHeap;
 
void *operator new(size_t size) {
    return userHeap::malloc(size);
}
 
void *operator new[](size_t size) {
    return userHeap::malloc(size);
}

void* operator new(size_t size, void* ptr) {
    return ptr;
}

void* operator new[](size_t size, void* ptr) {
    return ptr;
}
 
void operator delete(void *p) {
    userHeap::free(p);
}
 
void operator delete[](void *p) {
    userHeap::free(p);
}

void operator delete(void* ptr, size_t size) {
    userHeap::free(ptr);
}

void operator delete[](void* ptr, size_t size) {
    userHeap::free(ptr);
}