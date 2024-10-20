.section ".text.first"

.global start
.type start, @function
start:
  mrs x13, MPIDR_EL1
  and x13, x13, 0xff
  cbnz x13, _ZN9Prekernel4haltEv

  msr SPSel, #0   
  ldr x14, =start
  mov sp, x14

  ldr x14, =start_of_bss
  ldr x15, =size_of_bss_divided_by_8
Lbss_clear_loop:
  str xzr, [x14], #8
  subs x15, x15, #1
  bne Lbss_clear_loop

  b init
