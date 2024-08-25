/**
 * @file cpu.h
 * @author Krisna Pranav
 * @brief CPU
 * @version 6.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/atomic.h>
#include <mods/badge.h>
#include <mods/noncopyable.h>
#include <mods/vector.h>
#include <mods/iterdecision.h>
#include <kernel/physical_address.h>
#include <kernel/virtual_address.h>

// PAGE_SIZE, GENERIC_INTERRUPT_HANDLERS_COUNT, PAGE_MASK
#define PAGE_SIZE 4096
#define GENERIC_INTERRUPT_HANDLERS_COUNT (256 - IRQ_VECTOR_BASE)
#define PAGE_MASK ((FlatPtr)0xfffff000u)

namespace Kernel {

    class MemoryManager;
    class PageDirectory;
    class PageTableEntry;

    struct [[gnu::packed]] DescriptorTablePointer {
        u16 limit;
        void* address;
    };

    struct [[gnu::packed]] TSS32 {
        u16 backlink, __blh;
        u32 esp0;
        u16 ss0, __ss0h;
        u32 esp1;
        u16 ss1, __ss1h;
        u32 esp2;
        u16 ss2, __ss2h;
        u32 cr3, eip, eflags;
        u32 eax, ecx, edx, ebx, esp, ebp, esi, edi;
        u16 es, __esh;
        u16 cs, __csh;
        u16 ss, __ssh;
        u16 ds, __dsh;
        u16 fs, __fsh;
        u16 gs, __gsh;
        u16 ldt, __ldth;
        u16 trace, iomapbase;
    };

    union [[gnu::packed]] Descriptor {
        struct {
            u16 limit_lo;
            u16 base_lo;
            u8 base_hi;
            u8 type : 4;
            u8 descriptor_type : 1;
            u8 dpl : 2;
            u8 segment_present : 1;
            u8 limit_hi : 4;
            u8 : 1;
            u8 zero : 1;
            u8 operation_size : 1;
            u8 granularity : 1;
            u8 base_hi2;
        };

        struct {
            u32 low;
            u32 high;
        };

        enum Type {
            Invalid = 0,
            AvailableTSS_16bit = 0x1,
            LDT = 0x2,
            BusyTSS_16bit = 0x3,
            CallGate_16bit = 0x4,
            TaskGate = 0x5,
            InterruptGate_16bit = 0x6,
            TrapGate_16bit = 0x7,
            AvailableTSS_32bit = 0x9,
            BusyTSS_32bit = 0xb,
            CallGate_32bit = 0xc,
            InterruptGate_32bit = 0xe,
            TrapGate_32bit = 0xf,
        };

        /**
         * @return void* 
         */
        void* get_base() const {
            u32 b = base_lo;
            b |= base_hi << 16;
            b |= base_hi2 << 24;
            return reinterpret_cast<void*>(b);
        }

        /**
         * @param b 
         */
        void set_base(void* b) {
            base_lo = (u32)(b)&0xffff;
            base_hi = ((u32)(b) >> 16) & 0xff;
            base_hi2 = ((u32)(b) >> 24) & 0xff;
        }

        /**
         * @param l 
         */
        void set_limit(u32 l) {
            limit_lo = (u32)l & 0xffff;
            limit_hi = ((u32)l >> 16) & 0xf;
        }
    };

    class PageDirectoryEntry {
    public:

        /**
         * @return const PageTableEntry* 
         */
        const PageTableEntry* page_table_base() const { 
            return reinterpret_cast<PageTableEntry*>(m_raw & 0xfffff000u); 
        }
        
        /**
         * @return PageTableEntry* 
         */
        PageTableEntry* page_table_base() { 
            return reinterpret_cast<PageTableEntry*>(m_raw & 0xfffff000u); 
        }
        
        /**
         * @param value 
         */
        void set_page_table_base(u32 value) {
            m_raw &= 0x8000000000000fffULL;
            m_raw |= value & 0xfffff000;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const { 
            return m_raw == 0; 
        }

        /**
         * @brief clear
         * 
         */
        void clear() { 
            m_raw = 0; 
        }

        /**
         * @return u64 
         */
        u64 raw() const { 
            return m_raw; 
        }

        /**
         * @param other 
         */
        void copy_from(Badge<PageDirectory>, const PageDirectoryEntry& other) { 
            m_raw = other.m_raw; 
        }

        /**
         * @brief FLAGS
         * 
         */
        enum Flags {
            Present = 1 << 0,
            ReadWrite = 1 << 1,
            UserSupervisor = 1 << 2,
            WriteThrough = 1 << 3,
            CacheDisabled = 1 << 4,
            Huge = 1 << 7,
            Global = 1 << 8,
            NoExecute = 0x8000000000000000ULL,
        };

        /**
         * @return true 
         * @return false 
         */
        bool is_present() const { 
            return raw() & Present; 
        }

        /**
         * @param b 
         */
        void set_present(bool b) { 
            set_bit(Present, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_user_allowed() const { 
            return raw() & UserSupervisor; 
        }

        /**
         * @param b 
         */
        void set_user_allowed(bool b) { 
            set_bit(UserSupervisor, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_huge() const { 
            return raw() & Huge; 
        }

        /**
         * @param b 
         */
        void set_huge(bool b) { 
            set_bit(Huge, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_writable() const { 
            return raw() & ReadWrite; 
        }

        /**
         * @param b 
         */
        void set_writable(bool b) { 
            set_bit(ReadWrite, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_write_through() const { 
            return raw() & WriteThrough; 
        }

        /**
         * @param b 
         */
        void set_write_through(bool b) { 
            set_bit(WriteThrough, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_cache_disabled() const { 
            return raw() & CacheDisabled; 
        }

        /**
         * @param b 
         */
        void set_cache_disabled(bool b) { 
            set_bit(CacheDisabled, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_global() const { 
            return raw() & Global; 
        }

        /**
         * @param b 
         */
        void set_global(bool b) { 
            set_bit(Global, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_execute_disabled() const { 
            return raw() & NoExecute; 
        }

        /**
         * @param b 
         */
        void set_execute_disabled(bool b) { 
            set_bit(NoExecute, b); 
        }

        /**
         * @param bit 
         * @param value 
         */
        void set_bit(u64 bit, bool value) {
            if (value)
                m_raw |= bit;
            else
                m_raw &= ~bit;
        }

    private:
        u64 m_raw;
    };

    class PageTableEntry {
    public:

        /**
         * @return void* 
         */
        void* physical_page_base() { 
            return reinterpret_cast<void*>(m_raw & 0xfffff000u); 
        }

        /**
         * @param value 
         */
        void set_physical_page_base(u32 value) {
            m_raw &= 0x8000000000000fffULL;
            m_raw |= value & 0xfffff000;
        }

        /**
         * @return u64 
         */
        u64 raw() const { 
            return (u32)m_raw; 
        }

        // FLAGS
        enum Flags {
            Present = 1 << 0,
            ReadWrite = 1 << 1,
            UserSupervisor = 1 << 2,
            WriteThrough = 1 << 3,
            CacheDisabled = 1 << 4,
            Global = 1 << 8,
            NoExecute = 0x8000000000000000ULL,
        };

        /**
         * @return true 
         * @return false 
         */
        bool is_present() const { 
            return raw() & Present; 
        }

        /**
         * @param b 
         */
        void set_present(bool b) { 
            set_bit(Present, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_user_allowed() const { 
            return raw() & UserSupervisor; 
        }

        /**
         * @param b 
         */
        void set_user_allowed(bool b) { 
            set_bit(UserSupervisor, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_writable() const { 
            return raw() & ReadWrite; 
        }

        /**
         * @param b 
         */
        void set_writable(bool b) { 
            set_bit(ReadWrite, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_write_through() const { 
            return raw() & WriteThrough; 
        }

        /**
         * @param b 
         */
        void set_write_through(bool b) { 
            set_bit(WriteThrough, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_cache_disabled() const { 
            return raw() & CacheDisabled; 
        }

        /**
         * @param b 
         */
        void set_cache_disabled(bool b) { 
            set_bit(CacheDisabled, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_global() const { 
            return raw() & Global; 
        }

        /**
         * @param b 
         */
        void set_global(bool b) { 
            set_bit(Global, b); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_execute_disabled() const { 
            return raw() & NoExecute; 
        }

        /**
         * 
         * @param b 
         */
        void set_execute_disabled(bool b) { 
            set_bit(NoExecute, b); 
        }
        
        /**
         * @return true 
         * @return false 
         */
        bool is_null() const { 
            return m_raw == 0; 
        }

        void clear() { 
            m_raw = 0; 
        }

        /**
         * @param bit 
         * @param value 
         */
        void set_bit(u64 bit, bool value) {
            if (value)
                m_raw |= bit;
            else
                m_raw &= ~bit;
        }

    private:
        u64 m_raw;
    };
    
    /**
     * @brief Construct a new static assert object
     * 
     */
    static_assert(sizeof(PageDirectoryEntry) == 8);
    static_assert(sizeof(PageTableEntry) == 8);

    class PageDirectoryPointerTable {
    public:
        /**
         * @param index 
         * @return PageDirectoryEntry* 
         */
        PageDirectoryEntry* directory(size_t index) {
            return (PageDirectoryEntry*)(raw[index] & ~0xfffu);
        }

        u64 raw[4];
    };

    class GenericInterruptHandler;
    struct RegisterState;
    
    /**
     * @return const DescriptorTablePointer& 
     */
    const DescriptorTablePointer& get_gdtr();

    /**
     * @return const DescriptorTablePointer& 
     */
    const DescriptorTablePointer& get_idtr();

    /**
     * @param number 
     * @param f 
     */
    void register_interrupt_handler(u8 number, void (*f)());

    /**
     * @param number 
     * @param f 
     */
    void register_user_callable_interrupt_handler(u8 number, void (*f)());

    /**
     * @param interrupt_number 
     * @return GenericInterruptHandler& 
     */
    GenericInterruptHandler& get_interrupt_handler(u8 interrupt_number);

    /**
     * @param number 
     */
    void register_generic_interrupt_handler(u8 number, GenericInterruptHandler&);

    void replace_single_handler_with_shared(GenericInterruptHandler&);
    void replace_shared_handler_with_single(GenericInterruptHandler&);

    /**
     * @param number 
     */
    void unregister_generic_interrupt_handler(u8 number, GenericInterruptHandler&);
    void flush_idt();

    /**
     * @param selector 
     */
    void load_task_register(u16 selector);

    /**
     * @param description 
     * @param signal 
     * @param out_of_memory 
     */
    void handle_crash(RegisterState&, const char* description, int signal, bool out_of_memory = false);

    /**
     * @param dest_ptr 
     * @param src_ptr 
     * @param n 
     * @param fault_at 
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool safe_memcpy(void* dest_ptr, const void* src_ptr, size_t n, void*& fault_at);

    /**
     * @param str 
     * @param max_n 
     * @param fault_at 
     * @return ssize_t 
     */
    [[nodiscard]] ssize_t safe_strnlen(const char* str, size_t max_n, void*& fault_at);

    /**
     * @param dest_ptr 
     * @param c 
     * @param n 
     * @param fault_at 
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool safe_memset(void* dest_ptr, int c, size_t n, void*& fault_at);

    #define LSW(x) ((u32)(x)&0xFFFF)
    #define MSW(x) (((u32)(x) >> 16) & 0xFFFF)
    #define LSB(x) ((x)&0xFF)
    #define MSB(x) (((x) >> 8) & 0xFF)

    #define cli() asm volatile("cli" :: \
                                : "memory")
    #define sti() asm volatile("sti" :: \
                                : "memory")
    #define memory_barrier() asm volatile("" :: \
                                            : "memory")

    /**
     * @return * u32 
     */
    inline u32 cpu_flags() {
        u32 flags;
        asm volatile(
            "pushf\n"
            "pop %0\n"
            : "=rm"(flags)::"memory");
        return flags;
    }

    /**
     * @param segment 
     */
    inline void set_fs(u32 segment) {
        asm volatile(
            "movl %%eax, %%fs" ::"a"(segment)
            : "memory");
    }

    /**
     * @param segment 
     */
    inline void set_gs(u32 segment) {
        asm volatile(
            "movl %%eax, %%gs" ::"a"(segment)
            : "memory");
    }

    /**
     * @return u32 
     */
    inline u32 get_fs() {
        u32 fs;
        asm("mov %%fs, %%eax"
            : "=a"(fs));
        return fs;
    }

    /**
     * @return u32 
     */
    inline u32 get_gs() {
        u32 gs;
        asm("mov %%gs, %%eax"
            : "=a"(gs));
        return gs;
    }

    /**
     * @param offset 
     * @return u32 
     */
    inline u32 read_fs_u32(u32 offset) {
        u32 val;
        asm volatile(
            "movl %%fs:%a[off], %k[val]"
            : [ val ] "=r"(val)
            : [ off ] "ir"(offset));
        return val;
    }

    /**
     * @param offset 
     * @param val 
     */
    inline void write_fs_u32(u32 offset, u32 val) {
        asm volatile(
            "movl %k[val], %%fs:%a[off]" ::[off] "ir"(offset), [ val ] "ir"(val)
            : "memory");
    }

    /**
     * @return true 
     * @return false 
     */
    inline bool are_interrupts_enabled() {
        return cpu_flags() & 0x200;
    }

    class InterruptFlagSaver {
    public:
        /**
         * @brief Construct a new Interrupt Flag Saver object
         * 
         */
        InterruptFlagSaver() {
            m_flags = cpu_flags();
        }

        /**
         * @brief Destroy the Interrupt Flag Saver object
         * 
         */
        ~InterruptFlagSaver() {
            if (m_flags & 0x200)
                sti();
            else
                cli();
        }

    private:
        u32 m_flags;
    };

    /**
     * @return true 
     * @return false 
     */
    inline bool cli_and_save_interrupt_flag() {
        u32 flags = cpu_flags();
        cli();
        return flags & 0x200;
    }

    /**
     * @param flag 
     */
    inline void restore_interrupt_flag(bool flag) {
        if (flag)
            sti();
        else
            cli();
    }

    class InterruptDisabler {
    public:
        /**
         * @brief Construct a new Interrupt Disabler object
         * 
         */
        InterruptDisabler() {
            m_flags = cpu_flags();
            cli();
        }

        /**
         * @brief Destroy the Interrupt Disabler object
         * 
         */
        ~InterruptDisabler() {
            if (m_flags & 0x200)
                sti();
        }

    private:
        u32 m_flags;
    };

    class NonMaskableInterruptDisabler {
    public:
        NonMaskableInterruptDisabler();
        ~NonMaskableInterruptDisabler();
    };

    // IRQ
    #define IRQ_VECTOR_BASE 0x50

    struct PageFaultFlags {
        enum Flags {
            NotPresent = 0x00,
            ProtectionViolation = 0x01,
            Read = 0x00,
            Write = 0x02,
            UserMode = 0x04,
            SupervisorMode = 0x00,
            ReservedBitViolation = 0x08,
            InstructionFetch = 0x10,
        };
    };

    class PageFault {
    public:
        /**
         * @brief Construct a new Page Fault object
         * 
         * @param code 
         * @param vaddr 
         */
        PageFault(u16 code, VirtualAddress vaddr) : m_code(code) , m_vaddr(vaddr) {}

        /**
         * @brief TYPE
         * 
         */
        enum class Type {
            PageNotPresent = PageFaultFlags::NotPresent,
            ProtectionViolation = PageFaultFlags::ProtectionViolation,
        };

        /**
         * @brief ACCESS
         * 
         */
        enum class Access {
            Read = PageFaultFlags::Read,
            Write = PageFaultFlags::Write,
        };

        /**
         * @return VirtualAddress 
         */
        VirtualAddress vaddr() const { 
            return m_vaddr; 
        }

        /**
         * @return u16 
         */
        u16 code() const { 
            return m_code; 
        }

        /**
         * @return Type 
         */
        Type type() const { 
            return (Type)(m_code & 1); 
        }

        /**
         * @return Access 
         */
        Access access() const { 
            return (Access)(m_code & 2); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_not_present() const { 
            return (m_code & 1) == PageFaultFlags::NotPresent; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_protection_violation() const { 
            return (m_code & 1) == PageFaultFlags::ProtectionViolation; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_read() const { 
            return (m_code & 2) == PageFaultFlags::Read; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_write() const { 
            return (m_code & 2) == PageFaultFlags::Write; 
        }

        /**
         * @brief UserMode
         * 
         * @return true 
         * @return false 
         */
        bool is_user() const { return (m_code & 4) == PageFaultFlags::UserMode; }

        /**
         * @brief SupervisioMode
         * 
         * @return true 
         * @return false 
         */
        bool is_supervisor() const { return (m_code & 4) == PageFaultFlags::SupervisorMode; }

        /**
         * @brief InstructionFetch
         * 
         * @return true 
         * @return false 
         */
        bool is_instruction_fetch() const { return (m_code & 8) == PageFaultFlags::InstructionFetch; }

    private:
        u16 m_code;
        VirtualAddress m_vaddr;
    };

    /**
     * @brief registerState
     * 
     */
    struct [[gnu::packed]] RegisterState {
        u32 ss;
        u32 gs;
        u32 fs;
        u32 es;
        u32 ds;
        u32 edi;
        u32 esi;
        u32 ebp;
        u32 esp;
        u32 ebx;
        u32 edx;
        u32 ecx;
        u32 eax;
        u16 exception_code;
        u16 isr_number;
        u32 eip;
        u32 cs;
        u32 eflags;
        u32 userspace_esp;
        u32 userspace_ss;
    };

    #define REGISTER_STATE_SIZE (19 * 4)
    static_assert(REGISTER_STATE_SIZE == sizeof(RegisterState));
    
    /**
     * @brief FpuState
     * 
     */
    struct [[gnu::aligned(16)]] FPUState {
        u8 buffer[512];
    };

    /**
     * @param address 
     * @return constexpr FlatPtr 
     */
    constexpr FlatPtr page_base_of(FlatPtr address) {
        return address & PAGE_MASK;
    }

    /**
     * @param address 
     * @return FlatPtr 
     */
    inline FlatPtr page_base_of(const void* address) {
        return page_base_of((FlatPtr)address);
    }

    /**
     * @param address 
     * @return constexpr FlatPtr 
     */
    constexpr FlatPtr offset_in_page(FlatPtr address) {
        return address & (~PAGE_MASK);
    }

    /**
     * @param address 
     * @return FlatPtr 
     */
    inline FlatPtr offset_in_page(const void* address) {
        return offset_in_page((FlatPtr)address);
    }

    u32 read_cr0();
    u32 read_cr3();
    void write_cr3(u32);
    u32 read_cr4();

    u32 read_dr6();

    /**
     * @return true 
     * @return false 
     */
    static inline bool is_kernel_mode() {
        u32 cs;
        asm volatile(
            "movl %%cs, %[cs] \n"
            : [ cs ] "=g"(cs));
        return (cs & 3) == 0;
    }

    class CPUID {
    public:
        CPUID(u32 function) { asm volatile("cpuid"
                                        : "=a"(m_eax), "=b"(m_ebx), "=c"(m_ecx), "=d"(m_edx)
                                        : "a"(function), "c"(0)); }

        /**
         * @return u32 
         */
        u32 eax() const { 
            return m_eax; 
        }

        /**
         * @return u32 
         */
        u32 ebx() const { 
            return m_ebx; 
        }

        /**
         * @return u32 
         */
        u32 ecx() const { 
            return m_ecx; 
        }

        /**
         * @return u32 
         */
        u32 edx() const { 
            return m_edx; 
        }

    private:
        u32 m_eax { 0xffffffff };
        u32 m_ebx { 0xffffffff };
        u32 m_ecx { 0xffffffff };
        u32 m_edx { 0xffffffff };
    };

    /**
     * @param lsw 
     * @param msw 
     */
    inline void read_tsc(u32& lsw, u32& msw) {
        asm volatile("rdtsc"
                    : "=d"(msw), "=a"(lsw));
    }

    /**
     * @return u64 
     */
    inline u64 read_tsc() {
        u32 lsw;
        u32 msw;
        read_tsc(lsw, msw);
        return ((u64)msw << 32) | lsw;
    }

    struct Stopwatch {
        /**
         * @brief splitqword
         * 
         */
        union SplitQword {
            struct {
                uint32_t lsw;
                uint32_t msw;
            };
            uint64_t qw { 0 };
        };

    public:
        /**
         * @param name 
         */
        Stopwatch(const char* name) : m_name(name)
        {
            read_tsc(m_start.lsw, m_start.msw);
        }

        /**
         * @brief Destroy the Stopwatch object
         * 
         */
        ~Stopwatch() {
            SplitQword end;
            read_tsc(end.lsw, end.msw);
            uint64_t diff = end.qw - m_start.qw;
            dbg() << "Stopwatch(" << m_name << "): " << diff << " ticks";
        }

    private:
        const char* m_name { nullptr };
        SplitQword m_start;
    };

    // CPU Feature
    enum class CPUFeature : u32 {
        NX = (1 << 0),
        PAE = (1 << 1),
        PGE = (1 << 2),
        RDRAND = (1 << 3),
        RDSEED = (1 << 4),
        SMAP = (1 << 5),
        SMEP = (1 << 6),
        SSE = (1 << 7),
        TSC = (1 << 8),
        RDTSCP = (1 << 9),
        CONSTANT_TSC = (1 << 10),
        NONSTOP_TSC = (1 << 11),
        UMIP = (1 << 12),
        SEP = (1 << 13),
        SYSCALL = (1 << 14),
        MMX = (1 << 15),
        SSE2 = (1 << 16),
        SSE3 = (1 << 17),
        SSSE3 = (1 << 18),
        SSE4_1 = (1 << 19),
        SSE4_2 = (1 << 20),
    };

    class Thread;
    struct TrapFrame;

    #define GDT_SELECTOR_CODE0 0x08
    #define GDT_SELECTOR_DATA0 0x10
    #define GDT_SELECTOR_CODE3 0x18
    #define GDT_SELECTOR_DATA3 0x20
    #define GDT_SELECTOR_TLS 0x28
    #define GDT_SELECTOR_PROC 0x30
    #define GDT_SELECTOR_TSS 0x38

    static_assert(GDT_SELECTOR_CODE0 + 8 == GDT_SELECTOR_DATA0); 

    static_assert(GDT_SELECTOR_CODE0 + 16 == GDT_SELECTOR_CODE3);
    static_assert(GDT_SELECTOR_CODE0 + 24 == GDT_SELECTOR_DATA3);

    class ProcessorInfo;
    class SchedulerPerProcessorData;
    struct MemoryManagerData;
    struct ProcessorMessageEntry;

    struct ProcessorMessage {
        enum Type {
            FlushTlb,
            Callback,
            CallbackWithData
        };
        Type type;
        volatile u32 refs; 
        union {
            ProcessorMessage* next; 
            struct {
                void (*handler)();
            } callback;
            struct {
                void* data;
                void (*handler)(void*);
                void (*free)(void*);
            } callback_with_data;
            struct {
                u8* ptr;
                size_t page_count;
            } flush_tlb;
        };

        volatile bool async;

        ProcessorMessageEntry* per_proc_entries;
    };

    struct ProcessorMessageEntry {
        ProcessorMessageEntry* next;
        ProcessorMessage* msg;
    };

    struct DeferredCallEntry {
        DeferredCallEntry* next;
        union {
            struct {
                void (*handler)();
            } callback;
            struct {
                void* data;
                void (*handler)(void*);
                void (*free)(void*);
            } callback_with_data;
        };
        bool have_data;
        bool was_allocated;
    };

    class Processor {
        friend class ProcessorInfo;

        MOD_MAKE_NONCOPYABLE(Processor);
        MOD_MAKE_NONMOVABLE(Processor);

        Processor* m_self; 
        
        DescriptorTablePointer m_gdtr;
        Descriptor m_gdt[256];
        u32 m_gdt_length;

        u32 m_cpu;
        u32 m_in_irq;
        u32 m_in_critical;

        TSS32 m_tss;
        static FPUState s_clean_fpu_state;
        CPUFeature m_features;
        static volatile u32 g_total_processors; 

        ProcessorInfo* m_info;
        MemoryManagerData* m_mm_data;
        SchedulerPerProcessorData* m_scheduler_data;
        Thread* m_current_thread;
        Thread* m_idle_thread;

        volatile ProcessorMessageEntry* m_message_queue; 

        bool m_invoke_scheduler_async;
        bool m_scheduler_initialized;
        Atomic<bool> m_halt_requested;

        DeferredCallEntry* m_pending_deferred_calls; // in reverse order
        DeferredCallEntry* m_free_deferred_call_pool_entry;
        DeferredCallEntry m_deferred_call_pool[5];

        void gdt_init();

        /**
         * @param selector 
         * @param low 
         * @param high 
         */
        void write_raw_gdt_entry(u16 selector, u32 low, u32 high);

        /**
         * @param selector 
         * @param descriptor 
         */
        void write_gdt_entry(u16 selector, Descriptor& descriptor);

        /**
         * @return Vector<Processor*>& 
         */
        static Vector<Processor*>& processors();

        /**
         * @param msg 
         */
        static void smp_return_to_pool(ProcessorMessage& msg);

        /**
         * @return ProcessorMessage& 
         */
        static ProcessorMessage& smp_get_from_pool();

        /**
         * @param msg 
         */
        static void smp_cleanup_message(ProcessorMessage& msg);

        /**
         * @param msg 
         * @return true 
         * @return false 
         */
        bool smp_queue_message(ProcessorMessage& msg);

        /**
         * @param cpu 
         * @param msg 
         * @param async 
         */
        static void smp_unicast_message(u32 cpu, ProcessorMessage& msg, bool async);

        /**
         * @param msg 
         */
        static void smp_broadcast_message(ProcessorMessage& msg);

        /**
         * @param msg 
         */
        static void smp_broadcast_wait_sync(ProcessorMessage& msg);
        static void smp_broadcast_halt();

        void deferred_call_pool_init();
        void deferred_call_execute_pending();
        DeferredCallEntry* deferred_call_get_free();
        void deferred_call_return_to_pool(DeferredCallEntry*);
        void deferred_call_queue_entry(DeferredCallEntry*);

        void cpu_detect();
        void cpu_setup();

        String features_string() const;

    public:
        Processor() = default;

        void early_initialize(u32 cpu);
        void initialize(u32 cpu);

        /**
         * @return u32 
         */
        static u32 count() {
            return g_total_processors;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void wait_check() {
            Processor::current().smp_process_pending_messages();
        }

        [[noreturn]] static void halt();

        static void flush_entire_tlb_local() {
            write_cr3(read_cr3());
        }

        /**
         * @param vaddr 
         * @param page_count 
         */
        static void flush_tlb_local(VirtualAddress vaddr, size_t page_count);

        /**
         * @param vaddr 
         * @param page_count 
         */
        static void flush_tlb(VirtualAddress vaddr, size_t page_count);

        Descriptor& get_gdt_entry(u16 selector);
        void flush_gdt();
        const DescriptorTablePointer& get_gdtr();

        static Processor& by_id(u32 cpu);

        static size_t processor_count() { return processors().size(); }

        template<typename Callback>
        static inline IterDecision for_each(Callback callback) {
            auto& procs = processors();
            size_t count = procs.size();
            for (size_t i = 0; i < count; i++) {
                if (callback(*procs[i]) == IterDecision::Break)
                    return IterDecision::Break;
            }
            return IterDecision::Continue;
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE ProcessorInfo& info() { 
            return *m_info; 
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE static Processor& current() {
            return *(Processor*)read_fs_u32(0);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static bool is_initialized() {
            return get_fs() == GDT_SELECTOR_PROC && read_fs_u32(0) != 0;
        }

        /**
         * @param scheduler_data 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void set_scheduler_data(SchedulerPerProcessorData& scheduler_data) {
            m_scheduler_data = &scheduler_data;
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE SchedulerPerProcessorData& get_scheduler_data() const {
            return *m_scheduler_data;
        }

        /**
         * @param mm_data 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void set_mm_data(MemoryManagerData& mm_data) {
            m_mm_data = &mm_data;
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE MemoryManagerData& get_mm_data() const {
            return *m_mm_data;
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE Thread* idle_thread() const {
            return m_idle_thread;
        }

        /**
         * @param idle_thread 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void set_idle_thread(Thread& idle_thread) {
            m_idle_thread = &idle_thread;
        }

        /**
         * @return ALWAYS_INLINE* 
         */
        ALWAYS_INLINE Thread* current_thread() const {
            ASSERT(&Processor::current() == this);
            return m_current_thread;
        }

        /**
         * @param current_thread 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void set_current_thread(Thread& current_thread) {
            m_current_thread = &current_thread;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 id() {
            return m_cpu;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 raise_irq() {
            return m_in_irq++;
        }

        /**
         * @param prev_irq 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void restore_irq(u32 prev_irq) {
            ASSERT(prev_irq <= m_in_irq);
            if (!prev_irq) {
                if (m_in_critical == 0) {
                    auto prev_critical = m_in_critical++;
                    m_in_irq = prev_irq;
                    deferred_call_execute_pending();
                    ASSERT(m_in_critical == prev_critical + 1);
                    m_in_critical = prev_critical;
                }
                if (!m_in_critical)
                    check_invoke_scheduler();
            } else {
                m_in_irq = prev_irq;
            }
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE u32& in_irq() {
            return m_in_irq;
        }

        /**
         * @param prev_flags 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void enter_critical(u32& prev_flags) {
            m_in_critical++;
            prev_flags = cpu_flags();
            cli();
        }

        /**
         * @param prev_flags 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void leave_critical(u32 prev_flags) {
            cli();
            ASSERT(m_in_critical > 0);
            if (m_in_critical == 1) {
                if (!m_in_irq) {
                    deferred_call_execute_pending();
                    ASSERT(m_in_critical == 1);
                }
                m_in_critical--;
                if (!m_in_irq)
                    check_invoke_scheduler();
            } else {
                m_in_critical--;
            }
            if (prev_flags & 0x200)
                sti();
            else
                cli();
        }

        /**
         * @param prev_flags 
         * @param enable_interrupts 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE u32 clear_critical(u32& prev_flags, bool enable_interrupts) {
            u32 prev_crit = m_in_critical;
            m_in_critical = 0;
            prev_flags = cpu_flags();
            if (!m_in_irq)
                check_invoke_scheduler();
            if (enable_interrupts)
                sti();
            return prev_crit;
        }

        /**
         * @param prev_crit 
         * @param prev_flags 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void restore_critical(u32 prev_crit, u32 prev_flags) {
            ASSERT(m_in_critical == 0);
            m_in_critical = prev_crit;
            if (prev_flags & 0x200)
                sti();
            else
                cli();
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE u32& in_critical() { 
            return m_in_critical; 
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const FPUState& clean_fpu_state() const {
            return s_clean_fpu_state;
        }

        static void smp_enable();
        bool smp_process_pending_messages();

        /**
         * @tparam Callback 
         * @param callback 
         * @param async 
         */
        template<typename Callback>
        static void smp_broadcast(Callback callback, bool async) {
            auto* data = new Callback(move(callback));
            smp_broadcast(
                [](void* data) {
                    (*reinterpret_cast<Callback*>(data))();
                },
                data,
                [](void* data) {
                    delete reinterpret_cast<Callback*>(data);
                },
                async);
        }

        /**
         * @param callback 
         * @param async 
         */
        static void smp_broadcast(void (*callback)(), bool async);

        /**
         * @param callback 
         * @param data 
         * @param free_data 
         * @param async 
         */
        static void smp_broadcast(void (*callback)(void*), void* data, void (*free_data)(void*), bool async);

        /**
         * @tparam Callback 
         * @param cpu 
         * @param callback 
         * @param async 
         */
        template<typename Callback>
        static void smp_unicast(u32 cpu, Callback callback, bool async) {
            auto* data = new Callback(move(callback));
            smp_unicast(
                cpu,
                [](void* data) {
                    (*reinterpret_cast<Callback*>(data))();
                },
                data,
                [](void* data) {
                    delete reinterpret_cast<Callback*>(data);
                },
                async);
        }

        /**
         * @param cpu 
         * @param callback 
         * @param async 
         */
        static void smp_unicast(u32 cpu, void (*callback)(), bool async);

        /**
         * @param cpu 
         * @param callback 
         * @param data 
         * @param free_data 
         * @param async 
         */
        static void smp_unicast(u32 cpu, void (*callback)(void*), void* data, void (*free_data)(void*), bool async);

        /**
         * @param vaddr 
         * @param page_count 
         */
        static void smp_broadcast_flush_tlb(VirtualAddress vaddr, size_t page_count);

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        static void deferred_call_queue(Callback callback) {
            auto* data = new Callback(move(callback));
            deferred_call_queue(
                [](void* data) {
                    (*reinterpret_cast<Callback*>(data))();
                },
                data,
                [](void* data) {
                    delete reinterpret_cast<Callback*>(data);
                });
        }

        /**
         * @param callback 
         */
        static void deferred_call_queue(void (*callback)());

        /**
         * @param callback 
         * @param data 
         * @param free_data 
         */
        static void deferred_call_queue(void (*callback)(void*), void* data, void (*free_data)(void*));

        /**
         * @param f 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool has_feature(CPUFeature f) const {
            return (static_cast<u32>(m_features) & static_cast<u32>(f)) != 0;
        }

        void check_invoke_scheduler();

        void invoke_scheduler_async() { 
            m_invoke_scheduler_async = true; 
        }

        /**
         * @param trap 
         * @param raise_irq 
         */
        void enter_trap(TrapFrame& trap, bool raise_irq);

        /**
         * @param trap 
         */
        void exit_trap(TrapFrame& trap);

        /**
         * @param initial_thread 
         */
        [[noreturn]] void initialize_context_switching(Thread& initial_thread);

        /**
         * @param from_thread 
         * @param to_thread 
         */
        void switch_context(Thread*& from_thread, Thread*& to_thread);

        /**
         * @param thread 
         * @param flags 
         */
        [[noreturn]] static void assume_context(Thread& thread, u32 flags);

        /**
         * @param thread 
         * @param leave_crit 
         * @return u32 
         */
        u32 init_context(Thread& thread, bool leave_crit);

        /**
         * @param thread 
         * @param max_frames 
         * @return Vector<FlatPtr> 
         */
        static Vector<FlatPtr> capture_stack_trace(Thread& thread, size_t max_frames = 0);

        /**
         * @param data 
         * @param len 
         */
        void set_thread_specific(u8* data, size_t len);
    };

    class ScopedCritical {
        MOD_MAKE_NONCOPYABLE(ScopedCritical);

    public: 
        /**
         * @brief Construct a new Scoped Critical object
         * 
         */
        ScopedCritical() {
            enter();
        }

        /**
         * @brief Destroy the Scoped Critical object
         * 
         */
        ~ScopedCritical() {
            if (m_valid)
                leave();
        }

        ScopedCritical(ScopedCritical&& from) : m_prev_flags(exchange(from.m_prev_flags, 0)) , m_valid(exchange(from.m_valid, false)) {}

        /**
         * @param from 
         * @return ScopedCritical& 
         */
        ScopedCritical& operator=(ScopedCritical&& from) {
            if (&from != this) {
                m_prev_flags = exchange(from.m_prev_flags, 0);
                m_valid = exchange(from.m_valid, false);
            }
            return *this;
        }

        /**
         * @brief leave trace
         * 
         */
        void leave() {
            ASSERT(m_valid);
            m_valid = false;
            Processor::current().leave_critical(m_prev_flags);
        }

        /**
         * @brief enter 
         * 
         */
        void enter() {
            ASSERT(!m_valid);
            m_valid = true;
            Processor::current().enter_critical(m_prev_flags);
        }

    private:
        u32 m_prev_flags { 0 };
        bool m_valid { false };
    };

    struct TrapFrame {
        u32 prev_irq_level;
        RegisterState* regs; 

        TrapFrame() = delete;
        TrapFrame(const TrapFrame&) = delete;
        TrapFrame(TrapFrame&&) = delete;
        TrapFrame& operator=(const TrapFrame&) = delete;
        TrapFrame& operator=(TrapFrame&&) = delete;
    };

    #define TRAP_FRAME_SIZE (2 * 4)
    
    /**
     * @brief Construct a new static assert object
     * 
     */
    static_assert(TRAP_FRAME_SIZE == sizeof(TrapFrame));

    extern "C" void enter_trap_no_irq(TrapFrame*);
    extern "C" void enter_trap(TrapFrame*);
    extern "C" void exit_trap(TrapFrame*);

    class MSR {
        uint32_t m_msr;

    public:
        /**
         * @return true 
         * @return false 
         */
        static bool have() {
            CPUID id(1);
            return (id.edx() & (1 << 5)) != 0;
        }

        /**
         * @brief Construct a new MSR object
         * 
         */
        MSR(const MSR&) = delete;
        MSR& operator=(const MSR&) = delete;

        MSR(uint32_t msr) : m_msr(msr) {}

        /**
         * @param low 
         * @param high 
         */
        void get(u32& low, u32& high) {
            asm volatile("rdmsr"
                        : "=a"(low), "=d"(high)
                        : "c"(m_msr));
        }

        /**
         * @param low 
         * @param high 
         */
        void set(u32 low, u32 high) {
            asm volatile("wrmsr" ::"a"(low), "d"(high), "c"(m_msr));
        }
    };

    /**
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void stac() {
        if (!Processor::current().has_feature(CPUFeature::SMAP))
            return;
        asm volatile("stac" ::
                        : "cc");
    }

    /**
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void clac() {
        if (!Processor::current().has_feature(CPUFeature::SMAP))
            return;
        asm volatile("clac" ::
                        : "cc");
    }

    class SmapDisabler {
    public:
        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE SmapDisabler() {
            m_flags = cpu_flags();
            stac();
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ~SmapDisabler() {
            if (!(m_flags & 0x40000))
                clac();
        }

    private:
        u32 m_flags;
    };

}