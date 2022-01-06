#pragma once

#include <core.h>

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

#define _define_vector_destroy(type, name)   \
	int name##_destroy(name vec) \
	{                                        \
		if (vec->destroyed)                  \
			return -1;                       \
                                             \
		vec->destroyed = 1;                  \
		eclib_free(vec->buffer);             \
		eclib_free(vec);                     \
		return 0;                            \
	}

#define _define_vector_wipe(type, name)                              \
	int name##_wipe(name vec, unsigned int index)        \
	{                                                                \
		if (!CHECK_INDEX(vec, index))                                \
			return -1;                                               \
                                                                     \
		eclib_memset(VECTOR_GET(vec, index, type), 0, sizeof(type)); \
		return 0;                                                    \
	}

#define _define_vector_copy(type, name)                                 \
	name name##_copy(name srcvec)                     \
	{                                                                   \
		name newvec = eclib_malloc(sizeof(struct vec_s_##name));  \
		newvec->capacity  = srcvec->capacity;                           \
		newvec->size      = srcvec->size;                               \
		newvec->destroyed = srcvec->destroyed;                          \
                                                                        \
		newvec->buffer = eclib_malloc(sizeof(type) * srcvec->capacity); \
		eclib_memcpy(newvec->buffer, srcvec->buffer,                    \
		             sizeof(type) * srcvec->capacity);                  \
                                                                        \
		return newvec;                                                  \
	}

#define _define_vector_reserve(type, name)                      \
	int name##_reserve(name vec, size_t amount)     \
	{                                                           \
		if (amount < vec->capacity)                             \
			return -1;                                          \
                                                                \
		int prevcap   = vec->capacity;                          \
		vec->capacity = amount;                                 \
                                                                \
		void *buf = eclib_malloc(sizeof(type) * vec->capacity); \
		eclib_memcpy(buf, vec->buffer, prevcap * sizeof(type)); \
		eclib_free(vec->buffer);                                \
                                                                \
		vec->buffer = buf;                                      \
		return 0;                                               \
	}

#define _define_vector_push(type, name)                                        \
	int name##_push(name vec, type object)                         \
	{                                                                          \
		if (vec->destroyed)                                                    \
			return -1;                                                         \
                                                                               \
		if (vec->size == vec->capacity)                                        \
			name##_reserve(vec, vec->capacity * 2);                      \
                                                                               \
		eclib_memcpy(VECTOR_GET(vec, vec->size, type), &object, sizeof(type)); \
                                                                               \
		return ++vec->size;                                                    \
	}

#define _define_vector_set(type, name)                                     \
	int name##_set(name vec, unsigned int index, type object)  \
	{                                                                      \
		if (vec->destroyed)                                                \
			return -1;                                                     \
                                                                           \
		if (!CHECK_INDEX(vec, index))                                      \
			return -1;                                                     \
                                                                           \
		eclib_memcpy(VECTOR_GET(vec, index, type), &object, sizeof(type)); \
		return 0;                                                          \
	}

#define _define_vector_pop(type, name)                         \
	type name##_pop(name vec)                      \
	{                                                          \
		vec->error = 0;                                        \
		if (vec->destroyed)                                    \
		{                                                      \
			_ret_error(ECLIB_EDESTR, vec, type);               \
		}                                                      \
                                                               \
		if (vec->size == 0)                                    \
		{                                                      \
			_ret_error(ECLIB_EIND, vec, type);                 \
		}                                                      \
                                                               \
		return *(type *) VECTOR_GET(vec, vec->size - 1, type); \
	}

#define _define_vector_iter(type, name)   \
	int name##_iter(name vec) \
	{                                     \
		if (vec->iter == (int) vec->size) \
		{                                 \
			vec->iter = 0;                \
			return -1;                    \
		}                                 \
                                          \
		return vec->iter++;               \
	}

#define _define_vector_find_int(type, name)                          \
	int name##_find_int(name vec, int offset, int value) \
	{                                                                \
		for (unsigned int i = 0; i < vec->size; i++)                 \
		{                                                            \
			type x = name##_get(vec, i);                       \
			if ((int) *(((char *) &x) + offset) == value)            \
				return i;                                            \
		}                                                            \
                                                                     \
		return -1;                                                   \
	}

#define define_vector(type, name) \
	define_vector_type(name);     \
	define_vector_functions(type, name);
