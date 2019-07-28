#include "idt.h"
#include "isr.h"
#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"

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
    set_idt_gate(0, (u32)(long)isr0);
    set_idt_gate(1, (u32)(long)isr1);
    set_idt_gate(2, (u32)(long)isr2);
    set_idt_gate(3, (u32)(long)isr3);
    set_idt_gate(4, (u32)(long)isr4);
    set_idt_gate(5, (u32)(long)isr5);
    set_idt_gate(6, (u32)(long)isr6);
    set_idt_gate(7, (u32)(long)isr7);
    set_idt_gate(8, (u32)(long)isr8);
    set_idt_gate(9, (u32)(long)isr9);
    set_idt_gate(10, (u32)(long)isr10);
    set_idt_gate(11, (u32)(long)isr11);
    set_idt_gate(12, (u32)(long)isr12);
    set_idt_gate(13, (u32)(long)isr13);
    set_idt_gate(14, (u32)(long)isr14);
    set_idt_gate(15, (u32)(long)isr15);
    set_idt_gate(16, (u32)(long)isr16);
    set_idt_gate(17, (u32)(long)isr17);
    set_idt_gate(18, (u32)(long)isr18);
    set_idt_gate(19, (u32)(long)isr19);
    set_idt_gate(20, (u32)(long)isr20);
    set_idt_gate(21, (u32)(long)isr21);
    set_idt_gate(22, (u32)(long)isr22);
    set_idt_gate(23, (u32)(long)isr23);
    set_idt_gate(24, (u32)(long)isr24);
    set_idt_gate(25, (u32)(long)isr25);
    set_idt_gate(26, (u32)(long)isr26);
    set_idt_gate(27, (u32)(long)isr27);
    set_idt_gate(28, (u32)(long)isr28);
    set_idt_gate(29, (u32)(long)isr29);
    set_idt_gate(30, (u32)(long)isr30);
    set_idt_gate(31, (u32)(long)isr31);

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

    set_idt_gate(32, (u32)(long)irq0);
    set_idt_gate(33, (u32)(long)irq1);
    set_idt_gate(34, (u32)(long)irq2);
    set_idt_gate(35, (u32)(long)irq3);
    set_idt_gate(36, (u32)(long)irq4);
    set_idt_gate(37, (u32)(long)irq5);
    set_idt_gate(38, (u32)(long)irq6);
    set_idt_gate(39, (u32)(long)irq7);
    set_idt_gate(40, (u32)(long)irq8);
    set_idt_gate(41, (u32)(long)irq9);
    set_idt_gate(42, (u32)(long)irq10);
    set_idt_gate(43, (u32)(long)irq11);
    set_idt_gate(44, (u32)(long)irq12);
    set_idt_gate(45, (u32)(long)irq13);
    set_idt_gate(46, (u32)(long)irq14);
    set_idt_gate(47, (u32)(long)irq15);

    set_idt();
}

void isr_handler(registers_t r)
{
    char int_str[3];
    kprint("Received Interrupt: ");
    int_to_ascii(r.int_no, int_str);
    kprint(int_str);
    kprint("\n");
    kprint(exception_msgs[r.int_no]);
    kprint("\n");
}

void register_int_hdlr(u8 n, isr_t hdlr)
{
   int_handlers[n] = hdlr;
}

#define EOI 0x20

void irq_handler(registers_t r)
{
    if (r.int_no >= 40)
        port_w8(PIC2_CMD, EOI);
    port_w8(PIC1_CMD, EOI);

    if (int_handlers[r.int_no] != 0) {
        isr_t hdlr = int_handlers[r.int_no];
        hdlr(r);
    }
}
