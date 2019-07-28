#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

void kmain(void)
{
    isr_install();
    __asm__ __volatile("sti");
    init_timer(50);
    init_keyboard();
}
