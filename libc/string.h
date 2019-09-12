#ifndef LIBC_STRING_H
#define LIBC_STRING_H

void int_to_ascii(int n, char *str);
void str_rev(char *s);
int str_len(char *s);
int str_cmp(char *s1, char *s2);
void append(char *s, char ch);
void backspace(char *s); 

#endif
