#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>

size_t strlen(const char *s);

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);

char *strcpy(char *s1, const char * s2);

char *strncpy(char *s1, const char *s2, size_t n);


void *memchr(const void *s, int c, size_t n);

void *memcpy(void *s1, const void *s2, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);

void *memmove(void *s1, const void *s2, size_t n);

#endif // __STRING_H
