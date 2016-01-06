#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#define PRINT_OPAQUE_STRUCT(p)  print_mem((p), sizeof(*(p)))

void print_mem(void const *vp, size_t n);

#ifdef __cplusplus
}
#endif

#endif
