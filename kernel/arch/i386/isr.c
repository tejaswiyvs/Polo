#include <kernel/isr.h>
#include <kernel/pic.h>
#include <stdio.h>
#include <log.h>
#include <kernel/tty.h>

void isr_irq_handler(register_t regs)
{
  char *output;
  int n = sprintf(output, "isr_irq: %d, %d\n", regs.int_no, regs.err_code);
  printf(output);

  if (regs.int_no >= 32) {
    pic_send_eoi(regs.int_no - 32);
  }

  return;
}
