#ifndef CPU_TIMER_H
#define CPU_TIMER_H

#include "types.h"

uint32_t timer_get_ticks();
void init_timer(uint32_t freq);

#endif
