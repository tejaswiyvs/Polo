#ifndef __pic_h
#define __pic_h

#include <stdint.h>

void setup_remap_pics();
void pic_send_eoi(uint8_t irq);

#endif
