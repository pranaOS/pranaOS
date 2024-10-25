/**
 * @file init.cpp
 * @author Krisna Pranav
 * @brief INIT
 * @version 6.0
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/types.h>
#include <kernel/arch/aarch64/asm_wrapper.h>
#include <kernel/prekernel/Arch/aarch64/aarch64_asm_utils.h>
#include <kernel/prekernel/Arch/aarch64/bootPPMParser.h>
#include <kernel/prekernel/Arch/aarch64/framebuffer.h>
#include <kernel/prekernel/Arch/aarch64/mailbox.h>
#include <kernel/prekernel/Arch/aarch64/prekernel.h>
#include <kernel/prekernel/Arch/aarch64/timer.h>
#include <kernel/prekernel/Arch/aarch64/uart.h>
#include <kernel/prekernel/Arch/aarch64/utils.h>

static void draw_logo();
static u32 query_firmware_version();

extern "C" void wait_cycles(int n);

struct TrapFrame 
{
    u64 x[31];     
    u64 spsr_el1;  
    u64 elr_el1;   
    u64 tpidr_el1; 
    u64 sp_el0;    
}; // struct TrapFrame

extern "C" [[noreturn]] void halt();
extern "C" [[noreturn]] void init();

/**
 * @return trap_frame
 * 
 */
extern "C" void exception_common(TrapFrame const* const trap_frame);

extern "C" [[noreturn]] void init()
{
    auto& uart = Prekernel::UART::the();

    uart.print_str("\r\nWelcome to pranaOS!\r\n");

    auto firmware_version = query_firmware_version();
    uart.print_str("Firmware version: ");
    uart.print_num(firmware_version);
    uart.print_str("\r\n");

    uart.print_str("CPU started in: EL");
    uart.print_num(static_cast<u64>(Kernel::Aarch64::Asm::get_current_exception_level()));
    uart.print_str("\r\n");

    uart.print_str("Drop CPU to EL1\r\n");
    Prekernel::drop_to_exception_level_1();

    extern uintptr_t vector_table_el1;
    el1_vector_table_install(&vector_table_el1);

    uart.print_str("Initialize MMU\r\n");
    Prekernel::init_prekernel_page_tables();

    auto& framebuffer = Prekernel::Framebuffer::the();
    if (framebuffer.initialized()) {
        draw_logo();
    }

    uart.print_str("Enter loop\r\n");

    auto& timer = Prekernel::Timer::the();
    u64 start_musec = 0;
    for (;;) {
        u64 now_musec;
        while ((now_musec = timer.microseconds_since_boot()) - start_musec < 1'000'000)
            ;
        start_musec = now_musec;
        uart.print_str("Timer: ");
        uart.print_num(now_musec);
        uart.print_str("\r\n");
    }
}

extern size_t __stack_chk_guard;
size_t __stack_chk_guard;

extern "C" [[noreturn]] void __stack_chk_fail();

void __stack_chk_fail()
{
    Prekernel::halt();
}

/**
 * @param int 
 */
[[noreturn]] void __assertion_failed(char const*, char const*, unsigned int, char const*)
{
    Prekernel::halt();
}

/**
 * @return trap_frame
 * 
 */
extern "C" void exception_common(TrapFrame const* const trap_frame)
{
    constexpr bool print_stack_frame = true;

    if constexpr (print_stack_frame) {
        auto& uart = Prekernel::UART::the();

        uart.print_str("Exception Generated\n");
        for (auto reg = 0; reg < 31; reg++) {
            uart.print_str("x");
            uart.print_num(reg);
            uart.print_str(": ");
            uart.print_hex(trap_frame->x[reg]);
            uart.print_str("\r\n");
        }

        uart.print_str("spsr_el1: ");
        uart.print_hex(trap_frame->spsr_el1);
        uart.print_str("\r\n");

        uart.print_str("elr_el1: ");
        uart.print_hex(trap_frame->elr_el1);
        uart.print_str("\r\n");

        uart.print_str("tpidr_el1: ");
        uart.print_hex(trap_frame->tpidr_el1);
        uart.print_str("\r\n");

        uart.print_str("sp_el0: ");
        uart.print_hex(trap_frame->sp_el0);
        uart.print_str("\r\n");
    }
}

class QueryFirmwareVersionMboxMessage : Prekernel::Mailbox::Message 
{
public:
    u32 version;

    /**
     * @brief Construct a new Query Firmware Version Mbox Message object
     * 
     */
    QueryFirmwareVersionMboxMessage()
        : Prekernel::Mailbox::Message(0x0000'0001, 4)
    {
        version = 0;
    }
}; // class QueryFirmwareVersionMboxMessage : Prekernel::Mailbox::Message 

static u32 query_firmware_version()
{
    struct __attribute__((aligned(16))) 
    {
        Prekernel::Mailbox::MessageHeader header;
        QueryFirmwareVersionMboxMessage query_firmware_version;
        Prekernel::Mailbox::MessageTail tail;
    } message_queue;

    if (!Prekernel::Mailbox::the().send_queue(&message_queue, sizeof(message_queue))) 
    {
        return 0xffff'ffff;
    }

    return message_queue.query_firmware_version.version;
} // static u32 query_firmware_version()

extern "C" const u32 pranaos_boot_logo_start;
extern "C" const u32 pranaos_boot_logo_size;

static void draw_logo()
{
    Prekernel::BootPPMParser logo_parser(reinterpret_cast<const u8*>(&pranaos_boot_logo_start), pranaos_boot_logo_size);

    if (!logo_parser.parse()) 
    {
        Prekernel::warnln("Invalid boot logo.");
        return;
    }

    auto& uart = Prekernel::UART::the();
    uart.print_str("Boot logo size: ");
    uart.print_num(pranaos_boot_logo_size);
    uart.print_str("\r\n");
    uart.print_str("Width: ");
    uart.print_num(logo_parser.image.width);
    uart.print_str("\r\n");
    uart.print_str("Height: ");
    uart.print_num(logo_parser.image.height);
    uart.print_str("\r\n");

    auto& framebuffer = Prekernel::Framebuffer::the();
    auto fb_ptr = framebuffer.gpu_buffer();
    auto image_left = (framebuffer.width() - logo_parser.image.width) / 2;
    auto image_right = image_left + logo_parser.image.width;
    auto image_top = (framebuffer.height() - logo_parser.image.height) / 2;
    auto image_bottom = image_top + logo_parser.image.height;
    auto logo_pixels = logo_parser.image.pixel_data;

    for (u32 y = 0; y < framebuffer.height(); y++) {
        for (u32 x = 0; x < framebuffer.width(); x++) {
            if (x >= image_left && x < image_right && y >= image_top && y < image_bottom) {
                switch (framebuffer.pixel_order()) {
                case Prekernel::Framebuffer::PixelOrder::RGB:
                    fb_ptr[0] = logo_pixels[0];
                    fb_ptr[1] = logo_pixels[1];
                    fb_ptr[2] = logo_pixels[2];
                    break;
                case Prekernel::Framebuffer::PixelOrder::BGR:
                    fb_ptr[0] = logo_pixels[2];
                    fb_ptr[1] = logo_pixels[1];
                    fb_ptr[2] = logo_pixels[0];
                    break;
                default:
                    Prekernel::warnln("Unsupported pixel format");
                    Prekernel::halt();
                }

                logo_pixels += 3;
            } else {
                fb_ptr[0] = 0xBD;
                fb_ptr[1] = 0xBD;
                fb_ptr[2] = 0xBD;
            }

            fb_ptr[3] = 0xFF;
            fb_ptr += 4;
        }
        fb_ptr += framebuffer.pitch() - framebuffer.width() * 4;
    }
} // static void draw_logo