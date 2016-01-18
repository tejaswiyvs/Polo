#import <stdlib.h>

/* Copyright (C) 1989, 1990, 1991, 1992 Free Software Foundation, Inc.
     Written by James Clark (jjc@jclark.com)

This file is part of groff.

groff is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

groff is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with groff; see the file COPYING.  If not, write to the Free Software
Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#define INT_DIGITS 19		/* enough for 64 bit integer */

char *itoa(i)
     int i;
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 2];
  char *p = buf + INT_DIGITS + 1;	/* points to terminating '\0' */
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			/* i < 0 */
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoau(unsigned int n, char s[])
{
    int i;

    i = 0;
    do {	   /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);	 /* delete it */
    s[i] = '\0';
    reverse(s);
}
void itoh(int n, char s[])
{
    int i;

    i = 0;
    do {	   /* generate digits in reverse order */
        if (n % 16 < 10) {
            s[i++] = n % 16 + '0';   /* get next digit */
        } else {
            s[i++] = n % 16 + '7';   /* get next digit */
        }
    } while ((n /= 16) > 0);	 /* delete it */
    s[i] = '\0';
    reverse(s);
}
int htoi(char s[])
{
    int i = 0;
    int v = 0;
    while (1 == 1) {
        int n = s[i];
        if (n < '0')
            return v;
        if (n > 'F')
            return v;
        if ((n > '9') && (n < 'A'))
            return v;
        v *= 16;
        if (n < 'A')
            v += n - '0';
        if (n > '9')
            v += n - '7';
        i++;
    }
}
