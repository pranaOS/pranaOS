//
//  osobject.h
//  libcpp
//
//  Created by KrisnaPranav on 11/03/22.
//

#pragma once

#include "osruntime.h"

typedef struct delement delement_t;

/**
 * @brief delement
 */
struct delement {
    void* data;
    delement_t* prev;
    delement_t* next;
};

/**
 * @brief list_t
 */
typedef struct {
    delement_t* head;
    delement_t* tail;
} list_t;

#define list_init() {0, 0}

/**
 * @brief list append elem
 */
static inline void list_append_elem(list_t* list, delement_t* elem) {
    elem->next = 0;
    elem->prev = list->tail;
    if (list->head == 0) {
        list->head = elem;
    } else {
        list->tail->next = elem;
    }
    list->tail = elem;
}

/**
 * @brief list append
 */
static inline delement_t* list_append(list_t* list, void* data) {
    delement_t* newElement = new delement_t();
    if (newElement) {
        newElement->data = data;
        list_append_elem(list, newElement);
    }
    
    return newElement;
}

/**
 * @brief osObject class
 */
class osObject {
public:
    virtual ~osObject();
    
protected:
    virtual void release();
    virtual osObject* alloc();
};
