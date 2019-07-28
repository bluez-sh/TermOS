#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"

static int print_char(char c, int row, int col, char attr);
static int get_cursor_offset();
static void set_cursor_offset(int offset);
static int get_offset(int row, int col);
static int get_row(int offset);
static int get_col(int offset);

/**** Public Kernel API functions ****/
void kprint_at(const char *msg, int row, int col)
{
    int offset, i;

    if (row >= 0 && col >= 0) {
        offset = get_offset(row, col);
    } else {
        offset = get_cursor_offset();
        row = get_row(offset);
        col = get_col(offset);
    }

    for (i = 0; msg[i] != '\0'; i++) {
        offset = print_char(msg[i], row, col, WHITE_ON_BLACK);
        row = get_row(offset);
        col = get_col(offset);
    }
}

void kprint(const char *msg)
{
    kprint_at(msg, -1, -1);
}

void clear_screen()
{
    unsigned char *vid_mem = (unsigned char*) VIDEO_ADDR;
    int screen_size = MAX_ROWS * MAX_COLS;

    int i;
    for (i = 0; i < screen_size; i++) {
        vid_mem[i<<1] = ' ';
        vid_mem[(i<<1)+1] = WHITE_ON_BLACK;
    }

    set_cursor_offset(get_offset(0, 0));
}

/**** Private Kernel functions ****/
static int print_char(char c, int row, int col, char attr)
{
    unsigned char *vid_mem = (unsigned char*) VIDEO_ADDR;
    int offset;

    if (row >= MAX_ROWS || col >= MAX_COLS) {
        vid_mem[2*(MAX_ROWS)*(MAX_COLS)-2] = 'E';
        vid_mem[2*(MAX_ROWS)*(MAX_COLS)-1] = RED_ON_WHITE;
        return get_offset(row, col);
    }

    if (row >= 0 && col >= 0)
        offset = get_offset(row, col);
    else
        offset = get_cursor_offset();

    if (!attr) attr = WHITE_ON_BLACK;

    if (c == '\n') {
        row = get_row(offset);
        offset = get_offset(row+1, 0);
    } else {
        vid_mem[offset] = c;
        vid_mem[offset+1] = attr;
        offset += 2;
    }

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++)
            mem_cpy(vid_mem + get_offset(i, 0),
                    vid_mem + get_offset(i-1, 0),
                    MAX_COLS << 1);
        
        unsigned char *last_line = vid_mem + get_offset(MAX_ROWS-1, 0);
        for (i = 0; i < (MAX_COLS << 1); i++)
            last_line[i] = 0;

        offset -= (MAX_COLS << 1);
    }

    set_cursor_offset(offset);
    return offset;
}

static int get_cursor_offset()
{
    short offset;
    port_w8(REG_SCR_CTRL, 14);
    offset = port_r8(REG_SCR_DATA) << 8;
    port_w8(REG_SCR_CTRL, 15);
    offset |= port_r8(REG_SCR_DATA);
    return offset << 1;
}

static void set_cursor_offset(int offset)
{
    offset = offset >> 1;
    port_w8(REG_SCR_CTRL, 14);
    port_w8(REG_SCR_DATA, (unsigned char)(offset >> 8));
    port_w8(REG_SCR_CTRL, 15);
    port_w8(REG_SCR_DATA, (unsigned char)(offset & 0xff));
}

static int get_offset(int row, int col)
{
    return (row * MAX_COLS + col) << 1;
}

static int get_row(int offset)
{
    return (offset >> 1) / MAX_COLS;
}

static int get_col(int offset)
{
    return (offset >> 1) % MAX_COLS;
}
