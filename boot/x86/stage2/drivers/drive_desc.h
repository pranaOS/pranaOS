// #ifndef STAGE2_DRIVE_DESC
// #define STAGE2_DRIVE_DESC

#pragma once

typedef struct {
    void* init;
    void* read;
    void* write;
} drive_desc_t;

// #endif // STAGE2_DRIVE_DESC