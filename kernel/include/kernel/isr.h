#ifndef __isr_h
#define __isr_h

#include <stdint.h>

struct Registers {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));

typedef struct Registers register_t;
typedef void (*isr_t)(registers_t);

void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_irq_handler(register_t regs);

#endif
