#include "util.h"
#include "../cpu/timer.h"

void delay(double sec)
{
    uint32_t start = timer_get_ticks();
    double ticks = sec * 50;
    while (timer_get_ticks() < start + ticks)
        ;
}
