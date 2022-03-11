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
