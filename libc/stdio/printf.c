#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static void print(const char* data, size_t data_length)
{
	for (size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while ( *format != '\0' )
	{
		if ( *format != '%' )
		{
		print_c:
			amount = 1;
			while (format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format;

		if ( *(++format) == '%' )
			goto print_c;

		if ( rejected_bad_specifier )
		{
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		if ( *format == 'c' )
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
		else if ( *format == 'd')
		{
			format++;
			int i = (int) va_arg(parameters, int);
			if (i == 0) {
				i += 48;
				print(&i, 1);
			}
			else {
				int j = i;
				int num_digits = 1;
				while (j != 0) {
					j /= 10;
					num_digits++;
				}

				int a[num_digits];
				int count = 0;
				while(i != 0) {
					j = i % 10;
					a[num_digits - count - 1] = j + 48;
					i /= 10;
					count++;
				}
				print(&a, num_digits);
			}
			// int d = (int) va_arg(parameters, int) + 48;
			// print(&d, sizeof(d));
		}
		else
		{
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}
