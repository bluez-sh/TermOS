#ifndef DRIVERS_KEYBOARD_H
#define DRIVERS_KEYBOARD_H

#include "../cpu/types.h"

struct kbd_state {
    uint8_t ctrl_held  : 1;
    uint8_t alt_held   : 1;
    uint8_t fn_held    : 1;
    uint8_t shift_held : 1;
    uint8_t caps_on    : 1;
    uint8_t numlk_on   : 1;
    uint8_t scrlk_on   : 1;
    uint8_t unused     : 1;
} kbd_state;

void init_keyboard();

#endif
