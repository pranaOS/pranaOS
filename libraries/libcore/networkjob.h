/**
 * @file networkjob.h
 * @author Krisna Pranav
 * @brief network job 
 * @version 6.0
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <libcore/object.h>

namespace Core
{
    class NetworkJob : public Object
    {
    
    protected:
        NetworkJob();

    private:
        RefPtr<NetworkResponse> m_response;
        Error m_error { Error::None };
    }; // class NetworkJob

    const char* to_string(NetworkJob::Error);
} // namespace Core