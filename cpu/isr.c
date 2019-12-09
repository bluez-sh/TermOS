#include "idt.h"
#include "isr.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"
#include "ports.h"
#include "timer.h"

#define PIC1_CMD 0x20
#define PIC1_DAT 0x21
#define PIC2_CMD 0xA0
#define PIC2_DAT 0xA1

isr_t int_handlers[256];

const char *exception_msgs[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_install()
{
    set_idt_gate(0, (uint32_t)(long)isr0);
    set_idt_gate(1, (uint32_t)(long)isr1);
    set_idt_gate(2, (uint32_t)(long)isr2);
    set_idt_gate(3, (uint32_t)(long)isr3);
    set_idt_gate(4, (uint32_t)(long)isr4);
    set_idt_gate(5, (uint32_t)(long)isr5);
    set_idt_gate(6, (uint32_t)(long)isr6);
    set_idt_gate(7, (uint32_t)(long)isr7);
    set_idt_gate(8, (uint32_t)(long)isr8);
    set_idt_gate(9, (uint32_t)(long)isr9);
    set_idt_gate(10, (uint32_t)(long)isr10);
    set_idt_gate(11, (uint32_t)(long)isr11);
    set_idt_gate(12, (uint32_t)(long)isr12);
    set_idt_gate(13, (uint32_t)(long)isr13);
    set_idt_gate(14, (uint32_t)(long)isr14);
    set_idt_gate(15, (uint32_t)(long)isr15);
    set_idt_gate(16, (uint32_t)(long)isr16);
    set_idt_gate(17, (uint32_t)(long)isr17);
    set_idt_gate(18, (uint32_t)(long)isr18);
    set_idt_gate(19, (uint32_t)(long)isr19);
    set_idt_gate(20, (uint32_t)(long)isr20);
    set_idt_gate(21, (uint32_t)(long)isr21);
    set_idt_gate(22, (uint32_t)(long)isr22);
    set_idt_gate(23, (uint32_t)(long)isr23);
    set_idt_gate(24, (uint32_t)(long)isr24);
    set_idt_gate(25, (uint32_t)(long)isr25);
    set_idt_gate(26, (uint32_t)(long)isr26);
    set_idt_gate(27, (uint32_t)(long)isr27);
    set_idt_gate(28, (uint32_t)(long)isr28);
    set_idt_gate(29, (uint32_t)(long)isr29);
    set_idt_gate(30, (uint32_t)(long)isr30);
    set_idt_gate(31, (uint32_t)(long)isr31);

    // Remap the PIC
    port_w8(PIC1_CMD, 0x11);
    port_w8(PIC2_CMD, 0x11);
    port_w8(PIC1_DAT, 0x20);
    port_w8(PIC2_DAT, 0x28);
    port_w8(PIC1_DAT, 0x04);
    port_w8(PIC2_DAT, 0x02);
    port_w8(PIC1_DAT, 0x01);
    port_w8(PIC2_DAT, 0x01);
    port_w8(PIC1_DAT, 0x0);
    port_w8(PIC2_DAT, 0x0);

    // Install IRQs

    set_idt_gate(32, (uint32_t)(long)irq0);
    set_idt_gate(33, (uint32_t)(long)irq1);
    set_idt_gate(34, (uint32_t)(long)irq2);
    set_idt_gate(35, (uint32_t)(long)irq3);
    set_idt_gate(36, (uint32_t)(long)irq4);
    set_idt_gate(37, (uint32_t)(long)irq5);
    set_idt_gate(38, (uint32_t)(long)irq6);
    set_idt_gate(39, (uint32_t)(long)irq7);
    set_idt_gate(40, (uint32_t)(long)irq8);
    set_idt_gate(41, (uint32_t)(long)irq9);
    set_idt_gate(42, (uint32_t)(long)irq10);
    set_idt_gate(43, (uint32_t)(long)irq11);
    set_idt_gate(44, (uint32_t)(long)irq12);
    set_idt_gate(45, (uint32_t)(long)irq13);
    set_idt_gate(46, (uint32_t)(long)irq14);
    set_idt_gate(47, (uint32_t)(long)irq15);

    set_idt();
}

void isr_handler(registers_t *r)
{
    char int_str[3];
    kprint("Received Interrupt: ");
    int_to_ascii(r->int_no, int_str);
    kprint(int_str);
    kprint("\n");
    kprint(exception_msgs[r->int_no]);
    kprint("\n");
}

void register_int_hdlr(uint8_t n, isr_t hdlr)
{
   int_handlers[n] = hdlr;
}

#define EOI 0x20

void irq_handler(registers_t *r)
{
    if (r->int_no >= 40)
        port_w8(PIC2_CMD, EOI);
    port_w8(PIC1_CMD, EOI);

    if (int_handlers[r->int_no] != 0) {
        isr_t hdlr = int_handlers[r->int_no];
        hdlr(r);
    }
}

void irq_install()
{
    __asm__ __volatile__("sti");
    init_timer(50);
    init_keyboard();
}
