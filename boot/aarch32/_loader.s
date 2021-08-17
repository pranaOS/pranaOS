.extern _start
.extern _start_secondary_cpu
.extern vector_table

.section ".pranaos_load_section"
pranaos_loader:
    ldr     sp, =STACK_PHYZ_TOP

    mrc     p15, #0, r0, c0, c0, #5
    and     r0, r0, #3
    cmp     r0, #0
    bne     secondary_cpu_loader
    push    {r4, r5, r6, r7, r8, r9}

    ldr     r0, =vector_table
    mcr     P15, 0, r0, c12, c0, 0

    @ mov     r1, #0x0000
    @ ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    @ stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
    @ ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8}
    @ stmia   r1!, {r2, r3, r4, r5, r6, r7, r8}
    pop     {r4, r5, r6, r7, r8, r9}

    ldr     r0, =_start
    add     r0, r0, #-1073741824
    add     r0, r0, #1048576
    mov     pc, r0

secondary_cpu_loader:
    ldr     sp, =STACK_SECONDARY_PHYZ_TOP
    mov     r9, #512
    mrc     p15, #0, r8, c0, c0, #5 // Read CPU ID register.
1:
    add     r8, r8, #1
    cmp     r8, #0
    beq     secondary_cpu_loader_exit
    add     r9, r9, #512
    b       1b
secondary_cpu_loader_exit:
    subs    sp, r9
    ldr     r0, =vector_table
    mcr     P15, 0, r0, c12, c0, 0

    ldr     r0, =_start_secondary_cpu
    add     r0, r0, #-1073741824
    add     r0, r0, #1048576
    mov     pc, r0
