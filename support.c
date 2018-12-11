// This saves over 1kb

#include <string.h>
#include <stdint.h>

void *memmove(void *dst0, const void *src0, size_t len) {
    char *dst = dst0;
    const char *src = src0;
    if (src < dst) {
        src += len;
        dst += len;
        while (len--)
            *--dst = *--src;
    } else if (src != dst) {
        while (len--)
            *dst++ = *src++;
    }

    return dst0;
}

void *memcpy(void *dst0, const void *src0, size_t len) __attribute__((alias("memmove")));

void *memset(void *s, int c, size_t n) {
    uint8_t *p = s;
    while (n--)
        *p++ = c;
    return s;
}

size_t strlen(const char *s) {
    const char *s0 = s;
    while (*s++)
        ;
    return (s - s0) - 1;
}

int memcmp(const void *aa, const void *bb, size_t n)
{
    const uint8_t *a = aa;
    const uint8_t *b = bb;
    while (n--) {
        if (*a != *b)
            return (int)*a - (int)*b;
    }
    return 0;
}
