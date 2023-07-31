/**
 * @file wait.cpp
 * @author Krisna Pranav
 * @brief wait
 * @version 1.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>
#include <kernel/api/syscall.h>

extern "C"
{
    /**
     * @param wstatus 
     * @return pid_t 
     */
    pid_t wait(int* wstatus)
    {
        return waitpid(-1, wstatus, 0);
    }

    pid_t waitpid(pid_t waitee, int* wstatus, int options)
    {
       siginfo_t siginfo;
       idtype_t idtype;
       id_t id;

       if (waitee < -1) {

       } else if (waitee == -1) {

       } else if (waitee == 0) {

       } else {
       } 

       
    }

}