#include "util.h"

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

void int_to_ascii(int n, char *str)
{
    int sign, i = 0;
    sign = n;
    if (n < 0) n = -n;
    do {
        str[i++] = n%10 + '0';
        n /= 10;
    } while (n);
    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
    str_rev(str);
}

void str_rev(char *s)
{
    int i, j, temp;
    for (i = 0, j = str_len(s)-1; i < j; i++, j--) {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

int str_len(char *s)
{
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}
