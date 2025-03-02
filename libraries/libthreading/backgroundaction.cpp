/**
 * @file backgroundaction.cpp
 * @author Krisna Pranav
 * @brief Background Action
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/queue.h>
#include <libthreading/backgroundaction.h>
#include <libthreading/mutex.h>
#include <libthreading/thread.h>
#include <unistd.h>

static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_condition = PTHREAD_COND_INITIALIZER;
static Queue<Function<void()>>* s_all_actions;
static Threading::Thread* s_background_thread;

static intptr_t background_thread_func()
{
    Vector<Function<void()>> actions;
    while (true) {

        pthread_mutex_lock(&s_mutex);

        while (s_all_actions->is_empty())
            pthread_cond_wait(&s_condition, &s_mutex);

        while (!s_all_actions->is_empty())
            actions.append(s_all_actions->dequeue());

        pthread_mutex_unlock(&s_mutex);

        for (auto& action : actions)
            action();

        actions.clear();
    }
}

static void init()
{
    s_all_actions = new Queue<Function<void()>>;
    s_background_thread = &Threading::Thread::construct(background_thread_func).leak_ref();
    s_background_thread->set_name("Background thread");
    s_background_thread->start();
}

/**
 * @return Threading::Thread& 
 */
Threading::Thread& Threading::BackgroundActionBase::background_thread()
{
    if (s_background_thread == nullptr)
        init();
    return *s_background_thread;
}

/**
 * @param work 
 */
void Threading::BackgroundActionBase::enqueue_work(Function<void()> work)
{
    if (s_all_actions == nullptr)
        init();

    pthread_mutex_lock(&s_mutex);
    s_all_actions->enqueue(move(work));
    pthread_cond_broadcast(&s_condition);
    pthread_mutex_unlock(&s_mutex);
}
