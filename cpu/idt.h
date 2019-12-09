#ifndef CPU_IDT_H
#define CPU_IDT_H

#include "types.h"

#define KERNEL_CS 0x08

/* Interrupt gate (handler) */
typedef struct {
    uint16_t addr_low;
    uint16_t seg_sel;
    uint8_t zero;
    /* Flag Bits
     * 7: is interrupt present 
     * 6-5: Privilege (0=kernel, 3=user) 
     * 4: 0 for IGs
     * 3-0: 1110 (14) for 32 bit IG */
    uint8_t flags;
    uint16_t addr_high;
} __attribute__((packed)) idt_gate_t;

/* Interrupt register */
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif
