/**
 * @file timer.h
 * @author Krisna Pranav
 * @brief timer
 * @version 6.0
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <libcore/object.h>

namespace Core 
{

    class Timer final : public Object 
    {
        C_OBJECT(Timer);

    public:
        /**
         * @brief Create a repeating object
         * 
         * @param interval_ms 
         * @param timeout_handler 
         * @param parent 
         * @return NonnullRefPtr<Timer> 
         */
        static NonnullRefPtr<Timer> create_repeating(int interval_ms, Function<void()>&& timeout_handler, Object* parent = nullptr)
        {
            auto timer = adopt_ref(*new Timer(interval_ms, move(timeout_handler), parent));
            timer->stop();
            return timer;
        }

        /**
         * @brief Create a single shot object
         * 
         * @param interval_ms 
         * @param timeout_handler 
         * @param parent 
         * @return NonnullRefPtr<Timer> 
         */
        static NonnullRefPtr<Timer> create_single_shot(int interval_ms, Function<void()>&& timeout_handler, Object* parent = nullptr)
        {
            auto timer = adopt_ref(*new Timer(interval_ms, move(timeout_handler), parent));
            timer->set_single_shot(true);
            timer->stop();
            return timer;
        }

        /**
         * @brief Destroy the Timer object
         * 
         */
        virtual ~Timer() override = default;

        void start();

        /**
         * @param interval_ms 
         */
        void start(int interval_ms);

        void restart();

        /**
         * @param interval_ms 
         */
        void restart(int interval_ms);
        void stop();

        void set_active(bool);

        bool is_active() const 
        { 
            return m_active; 
        }

        int interval() const 
        { 
            return m_interval_ms; 
        }

        /**
         * @brief Set the interval Object
         * 
         * @param interval_ms 
         */
        void set_interval(int interval_ms)
        {
            if (m_interval_ms == interval_ms)
                return;
            m_interval_ms = interval_ms;
            m_interval_dirty = true;
        }

        bool is_single_shot() const 
        { 
            return m_single_shot; 
        }

        /**
         * @brief Set the single shot object
         * 
         * @param single_shot 
         */
        void set_single_shot(bool single_shot) 
        { 
            m_single_shot = single_shot; 
        }

        Function<void()> on_timeout;

    private:
        /**
         * @brief Construct a new Timer object
         * 
         * @param parent 
         */
        explicit Timer(Object* parent = nullptr);

        /**
         * @brief Construct a new Timer object
         * 
         * @param interval_ms 
         * @param timeout_handler 
         * @param parent 
         */
        Timer(int interval_ms, Function<void()>&& timeout_handler, Object* parent = nullptr);

        virtual void timer_event(TimerEvent&) override;

        bool m_active { false };
        bool m_single_shot { false };
        bool m_interval_dirty { false };
        int m_interval_ms { 0 };
    }; // class Timer final : public Object 

} // namespace Core
