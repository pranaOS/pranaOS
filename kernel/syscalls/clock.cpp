/**
 * @file clock.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief clock
 * @version 6.0
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/time.h>
#include <kernel/process.h>
#include <kernel/time/timemanagement.h>

namespace Kernel 
{

    /**
     * @brief clock gettime in sys
     * 
     */
    int Process::sys$clock_gettime(clockid_t clock_id, Userspace<timespec*> user_ts)
    {
        REQUIRE_PROMISE(stdio);

        auto ts = TimeManagement::the().current_time(clock_id);

        if (ts.is_error())
            return ts.error();

        if (!copy_to_user(user_ts, &ts.value()))
            return -EFAULT;
            
        return 0;
    }

    /**
     * @brief clock settime in sys
     * 
     */
    int Process::sys$clock_settime(clockid_t clock_id, Userspace<const timespec*> user_ts)
    {
        REQUIRE_PROMISE(settime);

        if (!is_superuser())
            return -EPERM;

        timespec ts;

        if (!copy_from_user(&ts, user_ts))
            return -EFAULT;

        switch (clock_id) {
        case CLOCK_REALTIME:
            TimeManagement::the().set_epoch_time(ts);
            break;
        default:
            return -EINVAL;
        }
        return 0;
    }

    /**
     * @brief clock nanosleep in sys
     * 
     */
    int Process::sys$clock_nanosleep(Userspace<const Syscall::SC_clock_nanosleep_params*> user_params)
    {
        REQUIRE_PROMISE(stdio);

        Syscall::SC_clock_nanosleep_params params;

        if (!copy_from_user(&params, user_params))
            return -EFAULT;

        timespec requested_sleep;

        if (!copy_from_user(&requested_sleep, params.requested_sleep))
            return -EFAULT;

        bool is_absolute = params.flags & TIMER_ABSTIME;

        switch (params.clock_id) {
        case CLOCK_MONOTONIC:
        case CLOCK_REALTIME: {
            bool was_interrupted;
            if (is_absolute) {
                was_interrupted = Thread::current()->sleep_until(params.clock_id, requested_sleep).was_interrupted();
            } else {
                timespec remaining_sleep;
                was_interrupted = Thread::current()->sleep(params.clock_id, requested_sleep, &remaining_sleep).was_interrupted();
                if (was_interrupted && params.remaining_sleep && !copy_to_user(params.remaining_sleep, &remaining_sleep))
                    return -EFAULT;
            }
            if (was_interrupted)
                return -EINTR;
            return 0;
        }
        default:
            return -EINVAL;
        }
    }

    /**
     * @brief adjtime in sys
     * 
     */
    int Process::sys$adjtime(Userspace<const timeval*> user_delta, Userspace<timeval*> user_old_delta)
    {
        if (user_old_delta) {
            timespec old_delta_ts = TimeManagement::the().remaining_epoch_time_adjustment();
            timeval old_delta;

            timespec_to_timeval(old_delta_ts, old_delta);

            if (!copy_to_user(user_old_delta, &old_delta))
                return -EFAULT;
        }

        if (user_delta) {
            REQUIRE_PROMISE(settime);

            if (!is_superuser())
                return -EPERM;

            timeval delta;

            if (!copy_from_user(&delta, user_delta))
                return -EFAULT;

            if (delta.tv_usec < 0 || delta.tv_usec >= 1'000'000)
                return -EINVAL;

            timespec delta_ts;
            timeval_to_timespec(delta, delta_ts);
            TimeManagement::the().set_remaining_epoch_time_adjustment(delta_ts);
        }

        return 0;
    }

    /**
     * @brief get time of day in sys
     * 
     */
    int Process::sys$gettimeofday(Userspace<timeval*> user_tv)
    {
        REQUIRE_PROMISE(stdio);

        auto tv = kgettimeofday();

        if (!copy_to_user(user_tv, &tv))
            return -EFAULT;

        return 0;
    }

} // namespace Kernel
