#include "util.h"
#include "../cpu/timer.h"

void delay(double sec)
{
    uint32_t start = timer_get_ticks();
    double ticks = sec * 50;
    // Somehow qemu disables interrupts here, so re-enable
    __asm__ volatile("sti");
    while (timer_get_ticks() < start + ticks)
        ;
}
