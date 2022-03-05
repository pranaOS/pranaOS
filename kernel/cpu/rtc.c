#include <cpu/pic.h>
#include <cpu/halt.h>
#include <cpu/rtc.h>
#include <cpu/idt.h>
#include <sys/time.h>
#include <ak/math.h>


/**
 * @brief current ticks
 * 
 */
static volatile uint64_t current_ticks = 0;

