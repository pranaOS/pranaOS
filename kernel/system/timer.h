#pragma once 

#include <locking/spinlock.h>
#include <stdint.h>
#include <libkernel/list.h>

#define TIMER_MAGIC 0x4b87ad6e

struct timer_list {
    uint64_t expires;
    void (*function)(struct timer_list *);
    struct list_head sibling;
    spinlock_t lock;
    uint32_t magic;
};

#define TIMER_INITIALIZER(_function, _expires) \
	{                                          \
		.function = (_function),               \
		.expires = (_expires),                 \
		.lock = 0,                             \
		.magic = TIMER_MAGIC                   \
	}

#define from_timer(var, callback_timer, timer_fieldname) \
	container_of(callback_timer, typeof(*var), timer_fieldname)

void add_timer(struct timer_list *timer);
void del_timer(struct timer_list *timer);