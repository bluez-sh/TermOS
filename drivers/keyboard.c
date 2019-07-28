#include "keyboard.h"
#include "ports.h"
#include "screen.h"
#include "../cpu/isr.h"
#include "../kernel/util.h"

static void print_key(u8 scancode)
{
    switch (scancode) {
    case 0x00: kprint("ERROR"); break;
    case 0x01: kprint("ESC"); break; 
    case 0x02: kprint("1"); break; 
    case 0x03: kprint("2"); break;
    case 0x04: kprint("3"); break;
    case 0x05: kprint("4"); break;
    case 0x06: kprint("5"); break;
    case 0x07: kprint("6"); break;
    case 0x08: kprint("7"); break;
    case 0x09: kprint("8"); break;
    case 0x0a: kprint("9"); break;
    case 0x0b: kprint("0"); break;
    case 0x0c: kprint("-"); break;
    case 0x0d: kprint("+"); break;
    case 0x0e: kprint("Backspace"); break;
    case 0x0f: kprint("Tab"); break;
    case 0x10: kprint("Q"); break;
    case 0x11: kprint("W"); break;
    case 0x12: kprint("E"); break;
    case 0x13: kprint("R"); break;
    case 0x14: kprint("T"); break;
    case 0x15: kprint("Y"); break;
    case 0x16: kprint("U"); break;
    case 0x17: kprint("I"); break;
    case 0x18: kprint("O"); break;
    case 0x19: kprint("P"); break;
    case 0x1a: kprint("["); break;
    case 0x1b: kprint("]"); break;
    case 0x1c: kprint("ENTER"); break;
    case 0x1d: kprint("LCtrl"); break;
    case 0x1e: kprint("A"); break;
    case 0x1f: kprint("S"); break;
    case 0x20: kprint("D"); break;
    case 0x21: kprint("F"); break;
    case 0x22: kprint("G"); break;
    case 0x23: kprint("H"); break;
    case 0x24: kprint("J"); break;
    case 0x25: kprint("K"); break;
    case 0x26: kprint("L"); break;
    case 0x27: kprint(";"); break;
    case 0x28: kprint("'"); break;
    case 0x29: kprint("`"); break;
    case 0x2a: kprint("LShift"); break;
    case 0x2b: kprint("\\"); break;
    case 0x2c: kprint("Z"); break;
    case 0x2d: kprint("X"); break;
    case 0x2e: kprint("C"); break;
    case 0x2f: kprint("V"); break;
    case 0x30: kprint("B"); break;
    case 0x31: kprint("N"); break;
    case 0x32: kprint("M"); break;
    case 0x33: kprint(","); break;
    case 0x34: kprint("."); break;
    case 0x35: kprint("/"); break;
    case 0x36: kprint("RShift"); break;
    case 0x37: kprint("Keypad *"); break;
    case 0x38: kprint("LAlt"); break;
    case 0x39: kprint("Space"); break;
    default:
               if (scancode <= 0x7f) {
                   kprint("Unknown key down");
               } else if (scancode <= 0x39 + 0x80) {
                   kprint("key up: ");
                   print_key(scancode-0x80);
               } else {
                   kprint("Unknown key up");
               }
               break;
    }
}

static void keyboard_callback(registers_t regs)
{
    char sc_ascii[8];
    u8 scancode = port_r8(0x60);
    int_to_ascii(scancode, sc_ascii);
    kprint("Keyboard scancode: ");
    kprint(sc_ascii);
    kprint(", ");
    print_key(scancode);
    kprint("\n");
}

void init_keyboard()
{
    register_int_hdlr(IRQ1, keyboard_callback);
}
