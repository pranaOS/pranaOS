/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Assertions.h>
#include <base/Atomic.h>
#include <base/Types.h>
#include <pthread.h>
#include <pranaos.h>

enum State : i32 {
    INITIAL = PTHREAD_ONCE_INIT,
    DONE,
    PERFORMING_NO_WAITERS,
    PERFORMING_WITH_WAITERS,
};

int pthread_once(pthread_once_t* self, void (*callback)(void))
{
    auto& state = reinterpret_cast<Atomic<State>&>(*self);

    State state2 = State::INITIAL;
    bool have_exchanged = state.compare_exchange_strong(
        state2, State::PERFORMING_NO_WAITERS, Base::memory_order_acquire);

    if (have_exchanged) {

        callback();

        state2 = state.exchange(State::DONE, Base::memory_order_release);
        switch (state2) {
        case State::INITIAL:
        case State::DONE:
            VERIFY_NOT_REACHED();
        case State::PERFORMING_NO_WAITERS:

            break;
        case State::PERFORMING_WITH_WAITERS:
            futex_wake(self, INT_MAX);
            break;
        }

        return 0;
    }

    while (true) {
        switch (state2) {
        case State::INITIAL:
            VERIFY_NOT_REACHED();
        case State::DONE:
            
            return 0;
        case State::PERFORMING_NO_WAITERS:

            have_exchanged = state.compare_exchange_strong(
                state2, State::PERFORMING_WITH_WAITERS, Base::memory_order_acquire);
            if (!have_exchanged) {
                
                continue;
            }
            state2 = State::PERFORMING_WITH_WAITERS;
            [[fallthrough]];
        case State::PERFORMING_WITH_WAITERS:

            futex_wait(self, state2, nullptr, 0);

            state2 = state.load(Base::memory_order_acquire);
            continue;
        }
    }
}
