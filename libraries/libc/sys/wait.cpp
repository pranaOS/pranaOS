/**
 * @file wait.cpp
 * @author Krisna Pranav
 * @brief wait
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <errno.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

/**
 * @param wstatus 
 * @return pid_t 
 */
pid_t wait(int* wstatus)
{
    return waitpid(-1, wstatus, 0);
}

/**
 * @param waitee 
 * @param wstatus 
 * @param options 
 * @return pid_t 
 */
pid_t waitpid(pid_t waitee, int* wstatus, int options)
{
    siginfo_t siginfo;
    idtype_t idtype;
    id_t id;

    if (waitee < -1) {
        idtype = P_PGID;
        id = -waitee;
    } else if (waitee == -1) {
        idtype = P_ALL;
        id = 0;
    } else if (waitee == 0) {
        idtype = P_PGID;
        id = getgid();
    } else {
        idtype = P_PID;
        id = waitee;
    }

    siginfo.si_pid = 0;
    int rc = waitid(idtype, id, &siginfo, options | WEXITED);

    if (rc < 0)
        return rc;

    if (wstatus) {
        if ((options & WNOHANG) && siginfo.si_pid == 0) {
            *wstatus = 0;
            return 0;
        }

        switch (siginfo.si_code) {
        case CLD_EXITED:
            *wstatus = siginfo.si_status << 8;
            break;
        case CLD_KILLED:
            *wstatus = siginfo.si_status;
            break;
        case CLD_STOPPED:
            *wstatus = siginfo.si_status << 8 | 0x7f;
            break;
        case CLD_CONTINUED:
            *wstatus = 0;
            return 0; 
        default:
            VERIFY_NOT_REACHED();
        }
    }

    return siginfo.si_pid;
}

/**
 * @param idtype 
 * @param id 
 * @param infop 
 * @param options 
 * @return int 
 */
int waitid(idtype_t idtype, id_t id, siginfo_t* infop, int options)
{
    Syscall::SC_waitid_params params { idtype, id, infop, options };
    int rc = syscall(SC_waitid, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

}
