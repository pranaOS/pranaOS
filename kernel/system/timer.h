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

/**
 * @brief add timer
 * 
 * @param timer 
 */
void add_timer(struct timer_list *timer);

/**
 * @brief delete timer
 * 
 * @param timer 
 */
void delete_timer(struct timer_list *timer);

/**
 * @brief mod timer
 * 
 * @param timer 
 * @param expires 
 */
void mod_timer(struct timer_list *timer, uint64_t expires);

/**
 * @brief is actived timer
 * 
 * @param timer 
 * @return true 
 * @return false 
 */
bool is_actived_timer(struct timer_list *timer);

/**
 * @brief timer init 
 * 
 */
void timer_init();