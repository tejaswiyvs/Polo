#ifndef __asm_h
#define __asm_h

#include <stdint.h>

/* A collection of assmebly snippets - Not sure where else to place them,
 * may get refactored out as I come up with a better design */


static inline void outb(uint16_t port, uint8_t val)
{
     asm volatile ( "outb %0, %w1" : : "a"(val), "d"(port) );
     /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
     /* TODO: Should %1 be %w1? */
}

static inline uint8_t inb(uint16_t port)
{
     uint8_t ret;
     asm volatile ( "inb %w1, %0" : "=a"(ret) : "d"(port) );
     /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
     /* TODO: Should %1 be %w1? */
     return ret;
}

static inline void io_wait(void)
{
     /* Port 0x80 is used for 'checkpoints' during POST. */
     /* The Linux kernel seems to think it is free for use :-/ */
     asm volatile ( "outb %%al, $0x80" : : "a"(0) );
     /* TODO: Is there any reason why al is forced? */
}

static inline void enable_interrupts(void)
{
  asm volatile ("sti");
}

static inline void disable_interrupts(void)
{
  asm volatile("cli");
}

#endif
