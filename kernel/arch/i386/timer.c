#include <kernel/timer.h>

void timer_init()
{
  register_interrupt_handler(32, &timer_int_handler);
}

void timer_ping(register_t regs)
{
  
}
