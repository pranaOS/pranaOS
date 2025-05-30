.section .text.vector_table

#define TRAP_FRAME_SIZE     272
#define SPSR_EL1_SLOT       (31 * 8)
#define ELR_EL1_SLOT        (32 * 8)
#define TPIDR_EL0_SLOT      (33 * 8)
#define SP_EL0_SLOT         (34 * 8)

.macro table_entry label
    .align 7
    b \label
.endm

.macro unimplemented_entry
    .align 7
    wfe
    b .
.endm

.extern exception_common

.macro save_current_context
    sub sp, sp, #TRAP_FRAME_SIZE

    stp x0, x1,     [sp, #(0 * 0)]
    stp x2, x3,     [sp, #(2 * 8)]
    stp x4, x5,     [sp, #(4 * 8)]
    stp x6, x7,     [sp, #(6 * 8)]
    stp x8, x9,     [sp, #(8 * 8)]
    stp x10, x11,   [sp, #(10 * 8)]
    stp x12, x13,   [sp, #(12 * 8)]
    stp x14, x15,   [sp, #(14 * 8)]
    stp x16, x17,   [sp, #(16 * 8)]
    stp x18, x19,   [sp, #(18 * 8)]
    stp x20, x21,   [sp, #(20 * 8)]
    stp x22, x23,   [sp, #(22 * 8)]
    stp x24, x25,   [sp, #(24 * 8)]
    stp x26, x27,   [sp, #(26 * 8)]
    stp x28, x29,   [sp, #(28 * 8)]
    str x30,        [sp, #(30 * 8)]

    mrs x0, spsr_el1
    str x0, [sp, #SPSR_EL1_SLOT]
    mrs x0, elr_el1
    str x0, [sp, #ELR_EL1_SLOT]
    mrs x0, tpidr_el0
    str x0, [sp, #TPIDR_EL0_SLOT]
    mrs x0, sp_el0
    str x0, [sp, #SP_EL0_SLOT]

    mov x0, sp
.endm

.macro restore_previous_context
    ldr x0, [sp, #SPSR_EL1_SLOT]
    msr spsr_el1, x0
    ldr x0, [sp, #ELR_EL1_SLOT]
    msr elr_el1, x0
    ldr x0, [sp, #TPIDR_EL0_SLOT]
    msr tpidr_el0, x0
    ldr x0, [sp, #SP_EL0_SLOT]
    msr sp_el0, x0

    ldp x0, x1,     [sp, #(0 * 0)]
    ldp x2, x3,     [sp, #(2 * 8)]
    ldp x4, x5,     [sp, #(4 * 8)]
    ldp x6, x7,     [sp, #(6 * 8)]
    ldp x8, x9,     [sp, #(8 * 8)]
    ldp x10, x11,   [sp, #(10 * 8)]
    ldp x12, x13,   [sp, #(12 * 8)]
    ldp x14, x15,   [sp, #(14 * 8)]
    ldp x16, x17,   [sp, #(16 * 8)]
    ldp x18, x19,   [sp, #(18 * 8)]
    ldp x20, x21,   [sp, #(20 * 8)]
    ldp x22, x23,   [sp, #(22 * 8)]
    ldp x24, x25,   [sp, #(24 * 8)]
    ldp x26, x27,   [sp, #(26 * 8)]
    ldp x28, x29,   [sp, #(28 * 8)]
    ldr x30,        [sp, #(30 * 8)]

    add sp, sp, #TRAP_FRAME_SIZE
.endm

.global vector_table_el1
.weak vector_table_el1 
.type vector_table_el1, @object

.align 11
vector_table_el1:
    unimplemented_entry
    unimplemented_entry
    unimplemented_entry
    unimplemented_entry

    table_entry synchronous_current_elsp_elx
    table_entry irq_current_elsp_elx
    table_entry fiq_current_elsp_elx
    table_entry system_error_current_elsp_elx

    unimplemented_entry
    unimplemented_entry
    unimplemented_entry
    unimplemented_entry

    unimplemented_entry
    unimplemented_entry
    unimplemented_entry
    unimplemented_entry

synchronous_current_elsp_elx:
    save_current_context
    bl exception_common
    restore_previous_context
    eret

irq_current_elsp_elx:
    save_current_context
    bl exception_common
    restore_previous_context
    eret

fiq_current_elsp_elx:
    save_current_context
    bl exception_common
    restore_previous_context
    eret

system_error_current_elsp_elx:
    save_current_context
    bl exception_common
    restore_previous_context
    eret
