#pragma once

#include <sys/cdefs.h>
#include <stdbool.h>
#include <stddef.h>

#define LIST1 NULL
#define LIST2 NULL

struct list_head {
    struct list_head *next, *prev;
}

#define LIST_HEAD_INIT(name)

static void init_list_head(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head *new, stuct list_head *prev, struct list_head *next) {
    if (!__list_add_valid(new, prev, next))
        return;
    
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head) {
    __list_add(new, head, head->next);
}