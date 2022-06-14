#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct node {
    struct node* next;
    struct node* prev;
    void* value;
    struct ListHeader * owner;
} __attribute__((packed)) node_t;

typedef struct ListHeader {
    node_t* head;
    node_t* tail;
    size_t length;
    const char* name;
    const void* metadata;
} __attribute__((packed)) list_t;

/**
 * @brief list destroy
 * 
 * @param list 
 */
extern void list_destroy(list_t* list);