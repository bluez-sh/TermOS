#include "mem.h"

void mem_cpy(void *src, const void *dst, unsigned nbytes)
{
    unsigned i;
    for (i = 0; i < nbytes; i++)
        *((u8*)dst + i) = *((u8*)src + i);
}

void mem_set(void *dst, int val, unsigned nbytes)
{
    unsigned i;
    for (i = 0; i < nbytes; i++)
        *((u8*)dst + i) = (u8)val;
}
