#ifndef __pit_h
#define __pit_h

#include <kernel/isr.h>

void pit_init();
void pit_fired(register_t regs);

#endif
