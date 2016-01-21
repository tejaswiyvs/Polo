#ifndef __keyboard_h
#define __keyboard_h

#include <stdint.h>

void keyboard_init(uint8_t port_id);
char getchar();

#endif
