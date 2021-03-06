#include <kernel/isr.h>
#include <kernel/pic.h>
#include <stdio.h>
#include <log.h>
#include <kernel/tty.h>

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void isr_irq_handler(register_t regs)
{
  if (interrupt_handlers[regs.int_no]) {
    interrupt_handlers[regs.int_no]();
  }

  if (regs.int_no >= 32 && regs.int_no <= 47) {
    pic_send_eoi(regs.int_no - 32);
  }

  return;
}
