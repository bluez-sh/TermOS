#ifndef DRIVERS_KEYBOARD_H
#define DRIVERS_KEYBOARD_H

#include "../cpu/types.h"

struct kbd_state {
    u8 ctrl_held  : 1;
    u8 alt_held   : 1;
    u8 fn_held    : 1;
    u8 shift_held : 1;
    u8 unused     : 4;
} kbd_state;

void init_keyboard();

#endif
