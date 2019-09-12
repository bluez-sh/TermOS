#ifndef LIBC_MEM_H 
#define LIBC_MEM_H

#include "../cpu/types.h"

void mem_cpy(void *src, const void *dst, unsigned nbytes);
void mem_set(void *dst, int val, unsigned nbytes);

#endif
