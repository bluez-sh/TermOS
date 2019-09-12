#include "keyboard.h"
#include "screen.h"
#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "../libc/string.h"
#include "../kernel/kernel.h"
#include "../libc/function.h"

#define BACKSPACE 0x0e
#define RETURN    0x1c
#define SHIFT     0x2a

#define MAX_SCANCODE 57

static char key_buffer[256];

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' ' };

const char sc_ascii_shift[] = { '?', '?', '!', '@', '#', '$', '%', '^', '&',
        '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ':', '\"', '~', '?', '|', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' ' };

const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
        "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
        "/", "RShift", "Keypad *", "LAlt", "Spacebar" };

static void keyboard_callback(registers_t regs)
{
    u8 scancode = port_r8(0x60);
    /*char sc[16];*/

    switch(scancode) {
        case SHIFT:
            kbd_state.shift_held = 1; 
            break;
        case SHIFT+0x80:
            kbd_state.shift_held = 0;
            break;
        case BACKSPACE:
            backspace(key_buffer);
            kprint_backspace();
            break;
        case RETURN:
            kprint("\n");
            user_input(key_buffer);
            key_buffer[0] = '\0';
            break;
        default: ;
    }

    if (scancode > MAX_SCANCODE) {
        /*int_to_ascii(scancode, sc);*/
        /*kprint("\nScancode: ");*/
        /*kprint(sc);*/
        return;
    } else {
        char key = kbd_state.shift_held ? 
                   sc_ascii_shift[(int)scancode]
                   : sc_ascii[(int)scancode];
        char str[2] = {key, '\0'};
        if (key != '?' || scancode == 0x35) {
            append(key_buffer, key);
            kprint(str);
        }

        /*int_to_ascii(scancode, sc);*/
        /*kprint("\nScancode: ");*/
        /*kprint(sc);*/
    }
    UNUSED(regs);
}

void init_keyboard()
{
    register_int_hdlr(IRQ1, keyboard_callback);
}
