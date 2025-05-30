#define __ASSEMBLY__
#include <frame.h>
#include <kernel_machine.h>
        
.macro global_func name
        .global \name
        .type \name STT_FUNC
        .size \name, \name\().end - \name
        .endm

.macro  frame_save el ex

        .if \el == 0
        str     x18, [sp, #-16] // stash user x18 and restore cpuinfo
        mrs     x18, tpidr_el1
        .endif

        str     x17, [sp, #-8]
        ldr     x17, [x18]      // current_context / frame
        stp     x0, x1, [x17, #(FRAME_X0 * 8)]
        stp     x2, x3, [x17, #(FRAME_X2 * 8)]
        stp     x4, x5, [x17, #(FRAME_X4 * 8)]
        stp     x6, x7, [x17, #(FRAME_X6 * 8)]
        stp     x8, x9, [x17, #(FRAME_X8 * 8)]
        stp     x10, x11, [x17, #(FRAME_X10 * 8)]
        stp     x12, x13, [x17, #(FRAME_X12 * 8)]
        stp     x14, x15, [x17, #(FRAME_X14 * 8)]
        ldr     x0, [sp, #-8]
        stp     x16, x0, [x17, #(FRAME_X16 * 8)]
        .if \el == 0
        ldr     x1, [sp, #-16]
        stp     x1, x19, [x17, #(FRAME_X18 * 8)]
        .else
        stp     x18, x19, [x17, #(FRAME_X18 * 8)]
        .endif
        stp     x20, x21, [x17, #(FRAME_X20 * 8)]
        stp     x22, x23, [x17, #(FRAME_X22 * 8)]
        stp     x24, x25, [x17, #(FRAME_X24 * 8)]
        stp     x26, x27, [x17, #(FRAME_X26 * 8)]
        stp     x28, x29, [x17, #(FRAME_X28 * 8)]
        .if \el == 0
        mrs     x1, sp_el0
        .else
        mov     x1, sp
        .endif
        mov     x0, #\el
        stp     x30, x1, [x17, #(FRAME_X30 * 8)]
        str     x0, [x17, #(FRAME_EL * 8)]

        .if \ex == 1
        mrs     x0, spsr_el1
        mrs     x1, esr_el1
        mrs     x2, elr_el1
        mrs     x3, far_el1
        .else
        mrs     x0, nzcv    
        
        mov     x2, #0x0
        .endif

        add     x17, x17, #256
        .if \ex == 1
        stp     w0, w1, [x17, #((FRAME_ESR_SPSR - 32) * 8)]
        str     x3, [x17, #((FRAME_FAULT_ADDRESS - 32) * 8)]
        .else
        str     w0, [x17, #((FRAME_ESR_SPSR - 32) * 8)]
        .endif
        str     x2, [x17, #((FRAME_ELR - 32) * 8)]
        .endm

        .text

.globl aarch64_cpu_init
aarch64_cpu_init:
        // allow debug and serror interrupts, disable irq and firq
        mov     x0, #0xc0
        msr     daif, x0

        // no trap on simd       
        mrs     x0, cpacr_el1
        orr     x0, x0, (CPACR_EL1_FPEN_NO_TRAP << CPACR_EL1_FPEN_SHIFT)
        msr     cpacr_el1, x0

        // temporary stack
        adr     x0, #0
        and     x0, x0, ~PHYSMEM_BASE_MASK
        add     sp, x0, INIT_IDENTITY_SIZE

        ret

// entry from kernel loader
.globl _start
_start:
        bl      aarch64_cpu_init
        b       start

// exception entries
entry_sync_el1h:
        frame_save 1 1
        b       synchronous_handler

entry_irq_el1h:
        frame_save 1 1
        b       irq_handler

entry_serror_el1h:
        frame_save 1 1
        b       serror_handler
        
entry_sync_el0: 
        frame_save 0 1
        b       synchronous_handler

entry_irq_el0:
        frame_save 0 1
        b       irq_handler

entry_serror_el0:
        frame_save 0 1
        b       serror_handler

entry_invalid_el0:
        frame_save 0 1
        b       invalid_handler

entry_invalid_el1:
        frame_save 1 1
        b       invalid_handler

// universal frame return
.globl frame_return
frame_return:
        mov     x1, #0x0        // clear frame full condition
        str     x1, [x0, #(FRAME_FULL * 8)]

        ldp     x30, x1, [x0, #(FRAME_X30 * 8)]
        ldp     x4, x5, [x0, #(FRAME_X4 * 8)]
        ldp     x6, x7, [x0, #(FRAME_X6 * 8)]
        ldp     x8, x9, [x0, #(FRAME_X8 * 8)]
        ldp     x10, x11, [x0, #(FRAME_X10 * 8)]
        ldp     x12, x13, [x0, #(FRAME_X12 * 8)]
        ldp     x14, x15, [x0, #(FRAME_X14 * 8)]
        ldp     x16, x17, [x0, #(FRAME_X16 * 8)]
        ldr     x19, [x0, #(FRAME_X19 * 8)]
        ldp     x20, x21, [x0, #(FRAME_X20 * 8)]
        ldp     x22, x23, [x0, #(FRAME_X22 * 8)]
        ldp     x24, x25, [x0, #(FRAME_X24 * 8)]
        ldp     x26, x27, [x0, #(FRAME_X26 * 8)]
        ldp     x28, x29, [x0, #(FRAME_X28 * 8)]
        ldp     x2, x3, [x0, #(FRAME_ESR_SPSR * 8)]
        cmp     x3, #0x0
        b.eq    3f

        mov     w2, w2          // mask off esr
        msr     spsr_el1, x2
        msr     elr_el1, x3
        tbnz    x2, #2, 1f
        
        ldr     x18, [x0, #(FRAME_X18 * 8)]
        msr     sp_el0, x1
        b       2f
        
1:      mov     sp, x1
2:      ldp     x2, x3, [x0, #(FRAME_X2 * 8)]
        ldp     x0, x1, [x0, #(FRAME_X0 * 8)]
        eret

        
3:      msr     nzcv, x2
        mov     sp, x1
        ldp     x2, x3, [x0, #(FRAME_X2 * 8)]
        ldp     x0, x1, [x0, #(FRAME_X0 * 8)]
        ret

.globl context_suspend
context_suspend:
        frame_save 1 0
        ldr     x0, [x18]
        b context_suspend_finish

.globl sysreg_get_id_aa64zfr0
sysreg_get_id_aa64zfr0:
        .long (INSN_MRS(0) | SYSREG_ID_AA64ZFR0_EL1)
        ret

        .globl arm_hvc
arm_hvc:
        // incomplete, just enough to issue power off
        hvc     #0
        ret

        .globl arm_hvc_full
arm_hvc_full:
        sub     sp, sp, #0x10
        stp     x8, x19, [sp]
        hvc     #0
        ldr     x19, [sp]
        stp     x0, x1, [x19]
        stp     x2, x3, [x19, #0x10]
        stp     x4, x5, [x19, #0x20]
        stp     x6, x7, [x19, #0x30]
        stp     x8, x9, [x19, #0x40]
        stp     x10, x11, [x19, #0x50]
        stp     x12, x13, [x19, #0x60]
        stp     x14, x15, [x19, #0x70]
        stp     x16, x17, [x19, #0x80]
        ldr     x19, [sp, #8]
        add     sp, sp, #0x10
        ret

.macro  vector  path
        .align 7
        b entry_\path
        .endm

// vector table
        .align 11
        .globl exception_vectors
exception_vectors:
        // EL1t
        vector  invalid_el1
        vector  invalid_el1
        vector  invalid_el1
        vector  invalid_el1

        // EL1h
        vector  sync_el1h
        vector  irq_el1h
        vector  invalid_el1
        vector  serror_el1h

        // EL0 - 64-bit
        vector  sync_el0
        vector  irq_el0
        vector  invalid_el0
        vector  serror_el0

        // EL0
        vector  invalid_el0
        vector  invalid_el0
        vector  invalid_el0
        vector  invalid_el0
        

.globl frame_save_fpsimd
frame_save_fpsimd:
        ldr     x0, [x0, #(FRAME_EXTENDED * 8)]
        stp     q0, q1, [x0, #(0 * 16)]
        stp     q2, q3, [x0, #(2 * 16)]
        stp     q4, q5, [x0, #(4 * 16)]
        stp     q6, q7, [x0, #(6 * 16)]
        stp     q8, q9, [x0, #(8 * 16)]
        stp     q10, q11, [x0, #(10 * 16)]
        stp     q12, q13, [x0, #(12 * 16)]
        stp     q14, q15, [x0, #(14 * 16)]
        stp     q16, q17, [x0, #(16 * 16)]
        stp     q18, q19, [x0, #(18 * 16)]
        stp     q20, q21, [x0, #(20 * 16)]
        stp     q22, q23, [x0, #(22 * 16)]
        stp     q24, q25, [x0, #(24 * 16)]
        stp     q26, q27, [x0, #(26 * 16)]
        stp     q28, q29, [x0, #(28 * 16)]
        stp     q30, q31, [x0, #(30 * 16)]
        mrs     x1, fpsr
        mrs     x2, fpcr
        add     x0, x0, #(32 * 16)
        stp     x1, x2, [x0]
1:      ret

.globl frame_restore_fpsimd
frame_restore_fpsimd:
        ldr     x0, [x0, #(FRAME_EXTENDED * 8)]
        ldp     q0, q1, [x0, #(0 * 16)]
        ldp     q2, q3, [x0, #(2 * 16)]
        ldp     q4, q5, [x0, #(4 * 16)]
        ldp     q6, q7, [x0, #(6 * 16)]
        ldp     q8, q9, [x0, #(8 * 16)]
        ldp     q10, q11, [x0, #(10 * 16)]
        ldp     q12, q13, [x0, #(12 * 16)]
        ldp     q14, q15, [x0, #(14 * 16)]
        ldp     q16, q17, [x0, #(16 * 16)]
        ldp     q18, q19, [x0, #(18 * 16)]
        ldp     q20, q21, [x0, #(20 * 16)]
        ldp     q22, q23, [x0, #(22 * 16)]
        ldp     q24, q25, [x0, #(24 * 16)]
        ldp     q26, q27, [x0, #(26 * 16)]
        ldp     q28, q29, [x0, #(28 * 16)]
        ldp     q30, q31, [x0, #(30 * 16)]
        add     x0, x0, #(32 * 16)
        ldr     x1, [x0]
        msr     fpsr, x1

    
        mrs     x2, fpcr
        ldr     x1, [x0, #8]
        cmp     x1, x2
        b.eq    1f
        msr     fpcr, x1
1:
        ret

global_func err_frame_save
err_frame_save:
        stp     x19, x20, [x0, #(ERR_FRAME_X19 * 8)]
        stp     x21, x22, [x0, #(ERR_FRAME_X21 * 8)]
        stp     x23, x24, [x0, #(ERR_FRAME_X23 * 8)]
        stp     x25, x26, [x0, #(ERR_FRAME_X25 * 8)]
        stp     x27, x28, [x0, #(ERR_FRAME_X27 * 8)]
        stp     x29, x30, [x0, #(ERR_FRAME_X29 * 8)]
        mov     x1, sp
        str     x1, [x0, #(ERR_FRAME_SP * 8)]
        mov     x0, #0
        ret
err_frame_save.end:

global_func err_frame_apply
err_frame_apply:
        ldp     x2, x3, [x0, #(ERR_FRAME_X19 * 8)]
        stp     x2, x3, [x1, #(FRAME_X19 * 8)]
        ldp     x2, x3, [x0, #(ERR_FRAME_X21 * 8)]
        stp     x2, x3, [x1, #(FRAME_X21 * 8)]
        ldp     x2, x3, [x0, #(ERR_FRAME_X23 * 8)]
        stp     x2, x3, [x1, #(FRAME_X23 * 8)]
        ldp     x2, x3, [x0, #(ERR_FRAME_X25 * 8)]
        stp     x2, x3, [x1, #(FRAME_X25 * 8)]
        ldp     x2, x3, [x0, #(ERR_FRAME_X27 * 8)]
        stp     x2, x3, [x1, #(FRAME_X27 * 8)]
        ldp     x2, x3, [x0, #(ERR_FRAME_X29 * 8)]
        str     x2, [x1, #(FRAME_X29 * 8)]
        str     x3, [x1, #(FRAME_ELR * 8)]
        ldr     x2, [x0, #(ERR_FRAME_SP * 8)]
        str     x2, [x1, #(FRAME_SP * 8)]
        mov     x0, #1  
        str     x0, [x1, #(FRAME_X0 * 8)]
        ret
err_frame_apply.end:

        .globl angel_shutdown
        .globl angel_shutdown_trap
angel_shutdown:
        str     x0, [sp, #8]
        mov     x1, #0x26         
        movk    x1, #0x2, lsl #16
        str     x1, [sp, #0]
        mov     x1, sp
        mov     w0, #0x18
angel_shutdown_trap:
        hlt     0xf000