#include <kernel/isr.h>
#include <kernel/pic.h>
#include <stdio.h>
#include <log.h>
#include <kernel/tty.h>

void isr_handler(register_t regs)
{
  terminal_writestring(itoa(regs.int_no));
  return;
}

void irq_handler(register_t regs)
{
  asm volatile ("xchgw %bx, %bx");
  logv("IRQ received!");
  signal_eoi(regs.int_no);
  /* char *output;
  int num_chars = sprintf(output, "Recieved int: %d", regs.int_no);
  printf(output); */
}
