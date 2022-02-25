/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _runtime_h_
#define _runtime_h_

/**
 * @brief objc class
 * 
 */
typedef struct objc_class *Class;

/**
 * @brief objc object
 * 
 */
typedef struct objc_object *id;

/**
 * @brief objc selector
 * 
 */
typedef struct objc_selector *SEL;

#ifdef __cplusplus
extern "C" {
#endif
    /**
     * @brief object get class
     * 
     * @param object 
     * @return Class 
     */
    Class object_getClass(id object);

    /**
     * @brief objc get class
     * 
     * @param aClassName 
     * @return Class 
     */
    Class objc_getClass(const char *aClassName);
    
#ifdef __cplusplus
}

#endif
#endif /* runtime_h */
