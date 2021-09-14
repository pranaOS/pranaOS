#pragma once

#ifndef __cplusplus

#   define __BEGIN_HEADER \
    extern "C"            \
    {

#   define __END_HEADER   \
    }

#else

#   define __BEGIN_HEADER \

#   define __END_HEADER   \

#endif 