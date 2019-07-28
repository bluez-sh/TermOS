#include "ports.h"

unsigned char port_r8(unsigned short port)
{
    unsigned char data;
    __asm__("in %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

void port_w8(unsigned short port, unsigned char data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_r16(unsigned short port)
{
    unsigned short data;
    __asm__("in %%dx, %%ax" : "=a" (data) : "d" (port));
    return data;
}

void port_w16(unsigned short port, unsigned short data)
{
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
