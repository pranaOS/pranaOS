//
// Created by KrisnaPranav on 27/01/22.
//

#pragma once

#include <stdint.h>

typedef struct {
    uint16_t kind;
    uint16_t info;
    char name[];
} ubsan_type_t;

typedef struct {
    uint32_t line;
    uint32_t column;
    const char* file;
} ubsan_source_location_t;

typedef struct {
    ubsan_source_location_t location
    ubsan_type_t* type;
    uint8_t align;
    uint8_t kind;
} ubsan_mismatch_data_t;

typedef struct {
    ubsan_source_location_t location;
    ubsan_type_t* type;
} ubsan_overflow_data_t;

typedef struct {
    ubsan_source_location_t location ;
    ubsan_type_t* lhsType;
    ubsan_type_t* rhsType;
} ubsan_shift_out_of_bounds_data_t;