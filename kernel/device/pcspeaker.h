/**
 * @file pcspeaker.h
 * @author Krisna Pranav aka (krishpranav)
 * @brief pc speaker
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once 

namespace Kernel
{

    class PCSpeaker 
    {
    public:
        static void tone_off();
        static void status();
        
    private:
        bool m_status { 0 };
    }; // class PCSpeaker

} // namespace Kernel