#ifndef DRIVERS_SCREEN_H
#define DRIVERS_SCREEN_H

#define VIDEO_ADDR 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

//Screen I/O ports
#define REG_SCR_CTRL 0x3d4
#define REG_SCR_DATA 0x3d5

//Public Kernel API
void kprint_at(const char *msg, int row, int col);
void kprint(const char *msg);
void clear_screen();

#endif
