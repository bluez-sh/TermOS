#ifndef LIBC_MEM_H 
#define LIBC_MEM_H

#include <stdint.h>
#include <stddef.h>

void mem_cpy(void *src, const void *dst, unsigned nbytes);
void mem_set(void *dst, int val, unsigned nbytes);

uint32_t kmalloc(size_t size);
void free(size_t size);

#endif
