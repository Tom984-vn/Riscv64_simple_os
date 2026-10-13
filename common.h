#pragma once

//typedef for redefine the keyword
                  //example change uint8_t to unsigned char
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef uint64_t size_t;
typedef uint64_t paddr_t;
typedef uint64_t vaddr_t;

#define true 1
#define false 0
#define NULL ((void *) 0)
#define align_up(value, align)    ((__typeof__(value))(((uint64_t)(value) + ((uint64_t)(align) - 1)) & ~((uint64_t)(align) - 1)))
#define is_aligned(value, align)  (((uint64_t)(value) & ((uint64_t)(align) - 1)) == 0)
#define offsetof(type, member)    __builtin_offsetof(type, member)
#define PAGE_SIZE 4096

#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

void *memset(void *buf, char c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
void printf(const char *fmt, ...);
