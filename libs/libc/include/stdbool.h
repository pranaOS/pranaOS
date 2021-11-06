#pragma once

#ifndef __cplusplus

#define bool _Bool
#define true 1
#define false 0

#else 

#define _Bool bool

#if __cplusplus < 201103L

#define bool bool
#define false false
#define true true
#endif

#endif 

#define __bool_true_false_are_defined 1