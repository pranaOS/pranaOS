/**
 * @file apic.h
 * @author Krisna Pranav
 * @brief apic
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>
#include <kernel/time/hardwaretimer.h>
#include <kernel/vm/memorymanager.h>

namespace Kernel 
{

    class APICTimer;

    struct LocalAPIC 
    {
        u32 apic_id;
    };

    class APIC 
    {
    public:
        /**
         * @return APIC& 
         */
        static APIC& the();

        /// @brief: initialize + initialized
        static void initialize();
        static bool initialized();

        bool init_bsp();
        void eoi();
        void boot_aps();

        /**
         * @param cpu 
         */
        void enable(u32 cpu);

        /**
         * @param cpu 
         */
        void init_finished(u32 cpu);

        void broadcast_ipi();

        /**
         * @param cpu 
         */
        void send_ipi(u32 cpu);

        static u8 spurious_interrupt_vector();

        /**
         * @param cpu 
         * @return Thread* 
         */
        Thread* get_idle_thread(u32 cpu) const;

        /**
         * @return u32 
         */
        u32 enabled_processor_count() const 
        { 
            return m_processor_enabled_cnt; 
        }

        /**
         * @return APICTimer* 
         */
        APICTimer* initialize_timers(HardwareTimerBase&);

        /**
         * @return APICTimer* 
         */
        APICTimer* get_timer() const 
        { 
            return m_apic_timer; 
        }

        enum class TimerMode 
        {
            OneShot,
            Periodic,
            TSCDeadline
        }; // TimerMode

        /// @brief: setup local timer
        void setup_local_timer(u32, TimerMode, bool);

        /**
         * @return u32 
         */
        u32 get_timer_current_count();
        u32 get_timer_divisor();

    private:
        class ICRReg 
        {
            u32 m_low { 0 };
            u32 m_high { 0 };

        public:
            enum DeliveryMode 
            {
                Fixed = 0x0,
                LowPriority = 0x1,
                SMI = 0x2,
                NMI = 0x4,
                INIT = 0x5,
                StartUp = 0x6,
            };

            enum DestinationMode 
            {
                Physical = 0x0,
                Logical = 0x1,
            };

            enum Level 
            {
                DeAssert = 0x0,
                Assert = 0x1
            };

            enum class TriggerMode 
            {
                Edge = 0x0,
                Level = 0x1,
            };

            enum DestinationShorthand 
            {
                NoShorthand = 0x0,
                Self = 0x1,
                AllIncludingSelf = 0x2,
                AllExcludingSelf = 0x3,
            };

            /**
             * @param vector 
             * @param delivery_mode 
             * @param destination_mode 
             * @param level 
             * @param trigger_mode 
             * @param destinationShort 
             * @param destination 
             */
            ICRReg(u8 vector, DeliveryMode delivery_mode, DestinationMode destination_mode, Level level, TriggerMode trigger_mode, DestinationShorthand destinationShort, u8 destination = 0)
                : m_low(vector | (delivery_mode << 8) | (destination_mode << 11) | (level << 14) | (static_cast<u32>(trigger_mode) << 15) | (destinationShort << 18))
                , m_high((u32)destination << 24)
            {
            }

            /**
             * @return u32 
             */
            u32 low() const 
            { 
                return m_low; 
            }

            u32 high() const 
            { 
                return m_high; 
            }
        };

        OwnPtr<Region> m_apic_base;
        Vector<OwnPtr<Processor>> m_ap_processor_info;
        Vector<Thread*> m_ap_idle_threads;
        AK::Atomic<u8> m_apic_ap_count { 0 };
        AK::Atomic<u8> m_apic_ap_continue { 0 };
        u32 m_processor_cnt { 0 };
        u32 m_processor_enabled_cnt { 0 };
        APICTimer* m_apic_timer { nullptr };

        static PhysicalAddress get_base();

        /**
         * @param base 
         */
        static void set_base(const PhysicalAddress& base);

        /**
         * @param offset 
         * @param value 
         */
        void write_register(u32 offset, u32 value);

        /**
         * @param offset 
         * @return u32 
         */
        u32 read_register(u32 offset);

        /**
         * @param offset 
         * @param interrupt 
         */
        void set_lvt(u32 offset, u8 interrupt);
        void set_siv(u32 offset, u8 interrupt);

        void wait_for_pending_icr();

        /**
         * @param icr 
         */
        void write_icr(const ICRReg& icr);
        void do_boot_aps();
    };

} // namespace Kernel
