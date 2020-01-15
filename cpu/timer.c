#include "timer.h"
#include "isr.h"
#include "ports.h"
#include "../drivers/screen.h"
#include "../libc/function.h"

volatile uint32_t tick = 0;

static void timer_callback(registers_t *regs)
{
    ++tick;
    UNUSED(regs);
}

uint32_t timer_get_ticks()
{
    return tick;
}

/* freq -> ticks per second */
void init_timer(uint32_t freq)
{
    register_int_hdlr(IRQ0, timer_callback);

    /* PIT value: hwclock @ 1193180 Hz */
    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor>>8) & 0xff);
    port_w8(0x43, 0x36);    /* Command port */
    port_w8(0x40, low);
    port_w8(0x40, high);
}
