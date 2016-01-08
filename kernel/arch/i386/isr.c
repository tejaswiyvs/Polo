#include "../include/kernel/isr.h"
#include <stdio.h>
#include <log.h>

void isr_handler(register_t regs)
{
  char *output;
  int num_chars = sprintf(output, "Recieved interrupt: %d", regs.int_no);
  printf(output);
}
