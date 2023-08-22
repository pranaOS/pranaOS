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

class PCSpeaker 
{
public:
    /**
     * @param frequency 
     */
    static void tone_on(int frequency);

    /// @brief: tone off
    static void tone_off();
}; // class PCSpeaker
