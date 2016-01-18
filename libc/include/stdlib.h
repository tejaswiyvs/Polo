#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);
char *itoa(int num);
int atoi(const char *str);
void itoau(unsigned int n, char s[]);
void itoh(int n, char s[]);
int htoi(char s[]);

#ifdef __cplusplus
}
#endif

#endif
