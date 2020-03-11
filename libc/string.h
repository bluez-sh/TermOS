#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include "../cpu/types.h"

void int_to_ascii(int n, char *str);
void hex_to_ascii(uint32_t n, char *str);
int ascii_to_int(char *str);

void str_rev(char *s);
int str_len(char *s);
int str_cmp(char *s1, char *s2);
int str_cmp_n(char *s1, char *s2, int n);
int str_cpy(char *dst, char *src);
void append(char *s, char ch);
void backspace(char *s);

#endif
