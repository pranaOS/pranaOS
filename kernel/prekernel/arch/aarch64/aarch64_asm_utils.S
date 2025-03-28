.global wait_cycles
.type wait_cycles, @function
wait_cycles:
Lstart:
  subs x0, x0, #1
  bne Lstart
  ret

.global enter_el2_from_el3
.type enter_el2_from_el3, @function
enter_el2_from_el3:
  adr x0, entered_el2
  msr elr_el3, x0
  eret
entered_el2:
  ret

.global enter_el1_from_el2
.type enter_el1_from_el2, @function
enter_el1_from_el2:
  adr x0, entered_el1
  msr elr_el2, x0
  eret
entered_el1:
  ret

.global el1_vector_table_install
.type el1_vector_table_install, @function
el1_vector_table_install:
    msr VBAR_EL1, x0
    ret
