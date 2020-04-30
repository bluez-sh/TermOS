#include "keyboard.h"
#include "screen.h"
#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../libc/stdio.h"
#include "../sched/sched.h"

#define BACKSPACE 0x0e
#define RETURN    0x1c
#define SHIFT     0x2a
#define LCTRL     0x1d

#define MAX_SCANCODE 57

static char key_buffer[512];
static int keys_in_buf = 0;

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

static void keyboard_callback(registers_t *regs)
{
    uint8_t scancode = port_r8(0x60);

    switch(scancode) {
        case SHIFT:
            kbd_state.shift_held = 1; 
            break;
        case SHIFT+0x80:
            kbd_state.shift_held = 0;
            break;
        case LCTRL:
            kbd_state.ctrl_held = 1;
            break;
        case LCTRL+0x80:
            kbd_state.ctrl_held = 0;
            break;
        case BACKSPACE:
            if (keys_in_buf > 0) {
                backspace(key_buffer);
                kprint_backspace();
                keys_in_buf--;
            }
            break;
        case RETURN:
            kprint("\n");
            copy_to_stdin(key_buffer);
            key_buffer[0] = '\0';
            keys_in_buf = 0;
            break;
        default: ;
    }

    if (scancode > MAX_SCANCODE) {
        if (scancode == 58)
            kbd_state.caps_on = !kbd_state.caps_on;
        /*kprint("\nScancode: ");*/
        /*kprintd(scancode);*/
    } else {
        char key = kbd_state.shift_held ? sc_ascii_shift[(int)scancode]
                                   : sc_ascii[(int)scancode];

        if (key >= 'a' && key <= 'z' && kbd_state.caps_on)
            key = sc_ascii_shift[(int)scancode];

        if (kbd_state.ctrl_held) {
            if (key == 'c' || key == 'C') {
                kbd_state.ctrl_held = 0;
                prompt_add_program();
            }
            return;
        }

        char str[2] = {key, '\0'};
        if (key != '?' || scancode == 0x35) {
            append(key_buffer, key);
            kprint(str);
            keys_in_buf++;
        }
        /*kprint("\nScancode: ");*/
        /*kprintd(scancode);*/
    }
    UNUSED(regs);
}

void init_keyboard()
{
    register_int_hdlr(IRQ1, keyboard_callback);
}
