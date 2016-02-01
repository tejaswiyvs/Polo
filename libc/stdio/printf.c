#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <log.h>

static void print(const char* data, size_t data_length)
{
	for (size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

int printf(const char* restrict format, ...)
{
  char output[500];
  va_list ap;
	va_start(ap, format);
	int n = vsprintf(output, format, ap);
	va_end(ap);
	print(output, strlen(output));
	return n;
}
