#include <kernel/timer.h>
#include <kernel/isr.h>

void timer_int_handler(register_t args);

void timer_init()
{
  register_interrupt_handler(32, &timer_int_handler);
}

void timer_int_handler(register_t regs)
{

}
