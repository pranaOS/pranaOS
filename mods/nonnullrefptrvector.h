/**
 * @file nonnullrefptrvector.h
 * @author Krisna Pranav
 * @brief nonnullrefptrvector
 * @version 6.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "nonnullptrvector.h"
#include "nonnullrefptrvector.h"
#include "nonnullrefptr.h"

namespace Mods
{
    template<typename T, int inline_capacity>
    class NonnullRefPtrVector : public NonnullPtrVector<NonnullRefPtr<T>, inline_capacity> 
    {
    }; // class

} // namespace Mods