#pragma once

size_t strlen(const char *string);
size_t strnlen(const char *s, size_t count);
char *strcpy(char *__restrict, const char *__restrict);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);
char *strchr(const char *, int);
char *strrchr(const char *, int);
void bcopy(const void *, void *, size_t);
void *memcpy(void *dest, const void *src, size_t n);
void *memcpyw(unsigned short *, const unsigned short *, size_t);
void *memset(void *, int, size_t);
void *memsetw(unsigned short *, unsigned short, size_t);

