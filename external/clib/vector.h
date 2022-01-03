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

#define define_vector_type(name) \
	struct vec_s_##name          \
	{                            \
		void * buffer;           \
		size_t size;             \
		size_t capacity;         \
		int    destroyed;        \
		int    error;            \
		int    iter;             \
	};                           \
	typedef struct vec_s_##name *name;

#define define_vector_functions(type, name) \
	_define_vector_create(type, name);      \
	_define_vector_get(type, name);         \
	_define_vector_destroy(type, name);     \
	_define_vector_wipe(type, name);        \
	_define_vector_copy(type, name);        \
	_define_vector_reserve(type, name);     \
	_define_vector_push(type, name);        \
	_define_vector_set(type, name);         \
	_define_vector_pop(type, name);         \
	_define_vector_iter(type, name);        \
	_define_vector_find_int(type, name);

#define _ret_error(err, vec, type) \
	vec->error = err;              \
	type x;                        \
	memset(&x, 0, sizeof(type));   \
	return x;

#define _define_vector_create(type, name)                              \
	name name##_create()                                   \
	{                                                                  \
		name vec = eclib_malloc(sizeof(struct vec_s_##name));    \
		vec->buffer    = eclib_malloc(sizeof(type) * VECTOR_STARTCAP); \
		vec->capacity  = VECTOR_STARTCAP;                              \
		vec->size      = 0;                                            \
		vec->destroyed = 0;                                            \
		vec->iter      = 0;                                            \
		vec->error     = 0;                                            \
		return vec;                                                    \
	}

#define _define_vector_get(type, name)                        \
	type name##_get(name vec, unsigned int index) \
	{                                                         \
		vec->error = 0;                                       \
		if (!CHECK_INDEX(vec, index))                         \
		{                                                     \
			_ret_error(ECLIB_EIND, vec, type);                \
		}                                                     \
		if (vec->destroyed)                                   \
		{                                                     \
			_ret_error(ECLIB_EDESTR, vec, type);              \
		}                                                     \
                                                              \
		return *(type *) VECTOR_GET(vec, index, type);        \
	}