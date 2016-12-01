#include <lib/cstring.h>

int strcmp(const char *string1, const char *string2) {
    if (string1 == 0 || string2 == 0) return 1;
    while (1) {
        if (*string1++ != *string2++)
            return 1;
        if (*string1 == '\0' && *string2 == '\0')
            return 0;
        if (*string1 == '\0' || *string2 == '\0')
            return 1;
    }
    return 0;
}

int strncmp(const char *cs, const char *ct, size_t count) {
    signed char __res = 0;
    while (count) {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
        count--;
    }
    return __res;
}

char *strrchr(const char *string, int c) {
    int i, len, last = -1;
    if (string == 0) return 0;
    len = strlen(string);
    for (i=0; i<len; i++) {
        if (string[i] == (char)c)
            last = i;
    }
    if (last != -1) return (char *)&string[last];
    return 0;
}

size_t strlen(const char *string) {
    char *temp;
    for (temp=(char *)string; *temp!=0; temp++);
    return temp-string;
}

size_t strnlen(const char *s, size_t maxlen) {
    const char *e;
    size_t n;
    for (e = s, n = 0; *e && n < maxlen; e++, n++);
    return n;
}

char *strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++) != 0);
    return dest;
}

char *strchr(const char *string, int c) {
    int i, len;
    if (string == 0) return 0;
    len = strlen(string);
    for (i=0; i<len; i++) {
        if (string[i] == (char)c)
            return (char *)&string[i];
    }
    return 0;

}

void *memcpy(void *dest, const void *src, size_t size) {
    size_t size4;
    size_t *d4, *s4;
    unsigned char *d1, *s1;
    for (size4 = size >> 2, d4 = (size_t *)dest, s4 = (size_t *)src;
         size4>0;
         size4--, *d4++ = *s4++
    );
    for (size = size % 4, d1 = (unsigned char *)d4, s1 = (unsigned char *)s4;
         size>0;
         size--, *d1++ = *s1++
    );
    return dest;

}

void *memcpyw(unsigned short *dest, const unsigned short *src, size_t count) {
    unsigned short *s, *d;
    for (s = (unsigned short *)src, d = (unsigned short *)dest;
         count != 0;
         count--, *d++ = *s++
    );
    return dest;

}

void *memset(void *ptr, int c, size_t size) {
    size_t i;
    if (ptr == 0) return 0;
    for (i=0; i<size; i++)
        ((char *)ptr)[i] = c;
    return ptr;

}

void *memsetw(unsigned short *dest, unsigned short val, size_t count) {
    unsigned short *temp;
    for (temp = (unsigned short *)dest;
         count != 0;
         count--, *temp++ = val
    );
    return dest;
}

