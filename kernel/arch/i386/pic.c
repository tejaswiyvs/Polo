#include <kernel/pic.h>
#include <asm.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define ICW1_ICW4      0x01 /* ICW4 (not) needed */
#define ICW1_SINGLE    0x02 /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL     0x08 /* Level triggered (edge) mode */
#define ICW1_INIT      0x10 /* Initialization - required! */

#define ICW4_8086       0x01 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM       0x10 /* Special fully nested (not) */

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

void setup_remap_pics()
{
  uint8_t a1, a2;

  // Save existing masks
  a1 = inb(PIC1_DATA);
  a2 = inb(PIC2_DATA);

  outb(PIC1_COMMAND, ICW1_INIT);
  io_wait();
  outb(PIC2_COMMAND, ICW1_INIT);
  io_wait();
  outb(PIC1_DATA, PIC1_OFFSET); // We're remapping the PICs interrupt codes from (0x07-0x7F) to (offset, offset + 8)
  io_wait();
  outb(PIC2_DATA, PIC2_OFFSET);
  io_wait();
  outb(PIC1_DATA, 4); // Tell the master PIC that there is a slave PIC at IRQ2 (00000100)
  io_wait();
  outb(PIC2_DATA, 2); // Tell the slave pic it's cascade identity (00000010)
  io_wait();

  outb(PIC1_DATA, ICW4_8086);
  io_wait();
  outb(PIC2_DATA, ICW4_8086);
  io_wait();

  // Restore saved masks
  outb(PIC1_DATA, a1);
  outb(PIC2_DATA, a2);

  enable_interrupts();
}

void pic_mask(uint8_t mask1, uint8_t mask2) {
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

// MARK :- Helpers
void signal_eoi(uint8_t irq)
{
  if (irq >= 8) {
    outb(PIC2_COMMAND, PIC_EOI);
  }

  // Always signal PIC1 that an interrupt has been handled
  // because it's the PIC that forwards PIC2's irqs as well.
  outb(PIC1_COMMAND, PIC_EOI);
}
