/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _objc_private_h_
#define _objc_private_h_

#ifndef _OBJC_OBJC_H
#error include objc-private.h
#endif

#define OBJC_TTYPES_DEFIEND 1
#define OBJC_OLD_DISPATCH_PROTOTYPES 1

#include <stdint.h>
#include <assert.h>

/**
 * @brief objc class
 * 
 */
struct objc_class;

/**
 * @brief objc object
 * 
 */
struct objc_object;

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
 * @brief side table 
 * 
 */
namespace {
    struct SideTable;
};

/**
 * @brief isa_t union func
 * 
 */
union isa_t {

};

struct objc_object {
public:
    /**
     * @brief isa 
     * 
     * @return Class 
     */
    Class ISA();

    /**
     * @brief Get the Isa object
     * 
     * @return Class 
     */
    Class getIsa();

    /**
     * @brief init isa
     * 
     * @param cls 
     */
    void initIsa(Class cls);

    /**
     * @brief int class isa
     * 
     * @param cls 
     */
    void initClassIsa(Class cls);

    /**
     * @brief initprotocolisa
     * 
     * @param cls 
     */
    void initProtocolIsa(Class cls);

    /**
     * @brief initInstanceISA
     * 
     * @param cls 
     * @param hasCxxDtor 
     */
    void initInstanceIsa(Class cls, bool hasCxxDtor);

private:
    /**
     * @brief isa 
     * 
     */
    isa_t isa;
};

#endif /* objc_private_h */
