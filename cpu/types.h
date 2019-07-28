#ifndef CPU_TYPES_H
#define CPU_TYPES_H

typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

#define low_16(addr) (u16)((addr) & 0xffff)
#define high_16(addr) (u16)(((addr) >> 16) & 0xffff)

#endif
