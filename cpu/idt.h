#ifndef CPU_IDT_H
#define CPU_IDT_H

#include "types.h"

#define KERNEL_CS 0x08

/* Interrupt gate (handler) */
typedef struct {
    u16 addr_low;
    u16 seg_sel;
    u8 zero;
    /* Flag Bits
     * 7: is interrupt present 
     * 6-5: Privilege (0=kernel, 3=user) 
     * 4: 0 for IGs
     * 3-0: 1110 (14) for 32 bit IG */
    u8 flags;
    u16 addr_high;
} __attribute__((packed)) idt_gate_t;

/* Interrupt register */
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, u32 handler);
void set_idt();

#endif
