#include "string.h"

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

void hex_to_ascii(uint32_t n, char *str)
{
    int i;
    append(str, '0');
    append(str, 'x');
    for (i = 7; i >= 0; i--) {
        int8_t nibble = (n & (0xF << (i*4))) >> i*4;
        if (nibble >= 0xA)
            append(str, nibble - 0xA + 'a');
        else
            append(str, nibble + '0');
    }
}

int ascii_to_int(char *str)
{
    int i = 0, val = 0;
    while (str[i] != '\0' &&
            str[i] >= '0' && str[i] <= '9')
        val = val * 10 + str[i++]-'0';
    return val;
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
    while (s[i] != '\0') i++;
    return i;
}

int str_cmp(char *s1, char *s2)
{
    int i;
    for (i = 0; s1[i] == s2[i]; i++)
        if (s1[i] == '\0') return 0;
    return s1[i] - s2[i];
}

int str_cmp_n(char *s1, char *s2, int n)
{
    int i;
    for (i = 0; s1[i] == s2[i]; i++)
        if (i == n-1) return 0;
    return s1[i] - s2[i];
}

int str_cpy(char *dst, char *src)
{
    int i;
    for (i = 0; src[i] != '\0'; i++)
        dst[i] = src[i];
    dst[i] = '\0';
    return i;
}

void append(char *s, char ch)
{
    int len = str_len(s);
    s[len] = ch;
    s[len+1] = '\0';
}

void backspace(char *s)
{
    int len = str_len(s);
    s[len-1] = '\0';
}
