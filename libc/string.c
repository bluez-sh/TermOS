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
