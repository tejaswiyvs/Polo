#include <stdio.h>

char *gets(buf)
	char *buf;
{
	register int c;
	register char *s;

	for (s = buf; (c = getchar()) != '\n';)
		if (c == 0x0)
			if (s == buf)
				return (NULL);
			else
				break;
		else {
			*s++ = c;
      putchar(c);
    }
	*s = 0;
	return (buf);
}
