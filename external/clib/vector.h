#pragma once

#include "core.h"


#define VECTOR_STARTCAP            8
#define CHECK_INDEX(vec, ind)      (ind < vec->size && !vec->destroyed)
#define VECTOR_GET(vec, ind, type) ((char *) vec->buffer + ind * sizeof(type))

#define define_vector_functionprototypes(type, name)                       \
	name name##_create();                                      \
	type       name##_get(name vec, unsigned int index);       \
	int        name##_destroy(name vec);                       \
	int        name##_wipe(name vec, unsigned int index);      \
	name name##_copy(name srcvec);                       \
	int        name##_reserve(name vec, size_t amount);        \
	int        name##_push(name vec, type object);             \
	type       name##_pop(name vec);                           \
	int name##_set(name vec, unsigned int index, type object); \
	int name##_iter(name vec);