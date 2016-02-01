#include <kernel/pit.h>
#include <kernel/isr.h>

// Base PIC frequency is 18.2065 Hz or once every ~54ms
// 18 -> 1 second.
// We can reprogram this to whatever frequency we want upto 1.19 Mhz
// We don't have much use for the PIT right now, so we skip this bit.

volatile uint32_t pit_count;

void pit_fired(register_t regs);

void pit_init()
{
  register_interrupt_handler(32, &pit_fired);
}

void pit_fired(register_t regs)
{
  pit_count++;
  if (pit_count == 18) {
    pit_count = 0;
  }
}
