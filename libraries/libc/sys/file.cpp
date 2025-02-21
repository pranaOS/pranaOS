/**
 * @file file.cpp
 * @author Krisna Pranav
 * @brief file
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

extern "C"
{
    int flock(int fd, int operation)
    {
        struct flock lock
        {

        };

        if (operation) {
            VERIFY_NOT_REACHED();
        }
        
    }
}