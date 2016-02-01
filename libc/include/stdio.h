#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NULL (void *)0

int snprintf(char* str, size_t size, const char* format, ...);
int sprintf(char* str, const char* format, ...);
int sscanf(const char* s, const char* format, ...);
int vsnprintf(char* str, size_t size, const char* format, va_list ap);
int vsprintf(char* str, const char* format, va_list ap);
int vsscanf(const char* s, const char* format, va_list ap);
int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
char *gets(char *buf);

#ifdef __cplusplus
}
#endif

#endif
