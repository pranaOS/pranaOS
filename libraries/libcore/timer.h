/**
 * @file timer.h
 * @author Krisna Pranav
 * @brief timer
 * @version 6.0
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <libcore/object.h>

namespace Core
{

    class Timer final : public Object
    {
    public:
        /**
         * @param interval 
         * @return NonnullRefPtr<Timer> 
         */
        static NonnullRefPtr<Timer> create_single_shot(int interval)
        {
            auto timer = adopt(*new Timer(interval, move(timeout_handler)));
            timer->set_single_shot(true);
            return timer;
        }

        /// @brief Destroy the Timer object
        virtual ~Timer() override;

        void start();
        /**
         * @param interval 
         */
        void start(int interval);

        void restart();
        /**
         * @param interval 
         */
        void restart(int interval);
        
        void stop();

    private:

        /**
         * @brief Construct a new Timer object
         * 
         * @param parent 
         */
        explicit Timer(Object* parent = nullptr);

        /**
         * @param interval 
         * @param timeout_handler 
         * @param parent 
         */
        Timer(int interval, Function<void()>&& timeout_handler, Object* parent = nullptr);

        virtual void timer_event(TimerEvent&) override;

        bool m_active { false };
        bool m_single_shot { false };
        bool m_interval_dirty { false };

        int m_interval { 0 };


    }; // class Timer

} // namespace Core