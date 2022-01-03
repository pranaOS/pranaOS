#pragma once

#include <stddef.h>
#include <sys/types.h>

#define NODESIZE_MAX 32

struct ll_node {
    size_t size;

    struct ll_node *next;
    struct ll_node *prev;
};

struct linkedlist {
    offset_t memstart;
    size_t maxsize;
    size_t cursize;
    
    int user;

    struct ll_node *start_used;
    struct ll_node *start_free;
};

struct linkedlist *create_linkedlist(offset_t memorystart, size_t maxsize,
                                    int user);

int free_node(struct ll_node *node, struct linkedlist *list);