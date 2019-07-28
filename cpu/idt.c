#include "idt.h"
#include "../kernel/util.h"

void set_idt_gate(int n, u32 handler)
{
    idt[n].addr_low = low_16(handler);
    idt[n].seg_sel = KERNEL_CS;
    idt[n].zero = 0;
    idt[n].flags = 0x8e;
    idt[n].addr_high = high_16(handler);
}

void set_idt()
{
    idt_reg.base = (u32) (long) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
