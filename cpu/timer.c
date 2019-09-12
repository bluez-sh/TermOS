#include "timer.h"
#include "isr.h"
#include "ports.h"
#include "../drivers/screen.h"
#include "../libc/function.h"

u32 tick = 0;

static void timer_callback(registers_t regs)
{
    ++tick;
    UNUSED(regs);
}

void init_timer(u32 freq)
{
    register_int_hdlr(IRQ0, timer_callback);

    /* PIT value: hwclock @ 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low = (u8)(divisor & 0xff);
    u8 high = (u8)((divisor>>8) & 0xff);
    port_w8(0x43, 0x36);    /* Command port */
    port_w8(0x40, low);
    port_w8(0x40, high);
}
