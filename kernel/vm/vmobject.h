/**
 * @file vmobject.h
 * @author Krisna Pranav
 * @brief vmobject
 * @version 1.0
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/lock.h>
#include <mods/inlinelinkedlist.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/typecasts.h>
#include <mods/weakable.h>
#include <mods/vector.h>