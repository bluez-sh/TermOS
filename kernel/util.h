#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H

#include "../cpu/types.h"

void mem_cpy(void *src, const void *dst, unsigned nbytes);
void mem_set(void *dst, int val, unsigned nbytes);
void int_to_ascii(int n, char *str);
void str_rev(char *s);
int str_len(char *s);

#endif
