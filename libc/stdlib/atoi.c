#include <ctype.h>

int atoi(const char* str) {
	int result = 0;
	while (isspace(*str))
		str++;
	while (isdigit(*str))
		result = (result * 10) + (*(str++) - '0');
	return result;
}
