#ifndef LIBC_UTIL_H
#define LIBC_UTIL_H

#include "../cpu/types.h"

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a > b) ? b : a)

void delay(uint32_t ms);

#endif
