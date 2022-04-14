#include "timer.h"
#include <cpu/idt.h>
#include <system/time.h>

static struct list_head list_of_timer;

/**
 * @brief asert timer valid
 * 
 * @param timer 
 */
static void assert_timer_valid(struct timer_list *timer) {
    if (timer->magic != TIMER_MAGIC)
        __asm__ __volatile("int $0x01");
}

/**
 * @brief is actived timer
 * 
 * @param timer 
 * @return true 
 * @return false 
 */
bool is_actived_timer(struct timer_list *timer) {
	return timer->sibling.prev != LIST1 && timer->sibling.next != LIST2;
}

/**
 * @brief add timer
 * 
 * @param timer 
 */
void add_timer(struct timer_list *timer) {
    struct timer_list *iter, *node = NULL;
    list_for_each_entry(iter, &list_of_timer, sibling) {
        assert_timer_valid(iter);
        if (timer->expires < iter->expires)
            break;
        
        node = iter;
    }

    if (node)
        list_add(&timer->sibling, &node->sibling);
    else
        list_add_tail(&timer->sibling, &list_of_timer);
}

void delete_timer(struct timer_list *timer) {
    list_del(&timer->sibling);
}

void mod_timer(struct timer_list *timer, uint64_t expires) {
    delete_timer(timer);
    timer->expires = expires;
    add_timer(timer);
}

static int32_t timer_schedule_handler(struct interrupt_registers *regs) {
	struct timer_list *iter, *next;
	uint64_t cms = get_milliseconds(NULL);
	list_for_each_entry_safe(iter, next, &list_of_timer, sibling)
	{
		assert_timer_valid(iter);
		if (iter->expires <= cms)
			iter->function(iter);
	}

	return IRQ_HANDLER_CONTINUE;
}
