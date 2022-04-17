#pragma once

#include <ak/types.h>

namespace Kernel {

    #define LOG_SHOW_MS 1

    enum logLevel {
        Info,
        Warning,
        Error
    };


    /**
     * example: 
        * @a sendLog(Info, "SomeInfo");
        * @b sendLog(Warning, "WARNING!!");
        * @c sendLog(Error, "ERROR!!")
        * 
     * @brief sendlog level
     * @param level 
     * @param format 
     * @param ...
     *  
     */
    void sendLog(logLevel level, const char* __restrict__ format, ...);

    /**
     * @brief print
     * 
     * @param data 
     * @param length 
     */
    void Print(const char* data, uint32_t length);        
}
