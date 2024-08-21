//
//  std.h
//  Libraries
//
//  Created by krisna pranav on 16/01/23.
//

#pragma once

#include <initializer_list>
#include <new>
#include <utility>
#include "_prelude.h"


#ifndef __ssize_t_defined

#    include <Meta/sign.h>

using ssize_t = Karm::Meta::MakeSigned<size_t>;

#endif
