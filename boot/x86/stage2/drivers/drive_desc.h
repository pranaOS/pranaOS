#pragma once

typedef struct {
    void* init;
    void* read;
    void* write;
} drive_desc_t;