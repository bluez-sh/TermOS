#include "util.h"
#include "../cpu/timer.h"

void delay(uint32_t ms)
{
    uint32_t start = timer_get_ticks();
    uint32_t ticks = (ms / 1000 * 50) + ((ms % 1000) * 50) / 1000;
    while (timer_get_ticks() < start + ticks)
        ;
}
