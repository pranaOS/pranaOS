#pragma once

#include <stddef.h>

void* operator new(size_t size) {
	return size;
}

void* operator new(size_t size, void* ptr) {
	return ptr;
}

void* operator new[](size_t size, void* ptr) {
	return ptr;
}