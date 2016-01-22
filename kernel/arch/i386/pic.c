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
#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

static void pic_mask(int pic_num, uint16_t mask);
static uint16_t pic_get_irq_reg(int ocw3);
static uint16_t pic_get_irr(void);
static uint16_t pic_get_isr(void);

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

  // Mask everything except the keyboard, timer
  pic_mask(1, 0xFC);
  pic_mask(2, 0xFF);
}

static void pic_mask(int pic_num, uint16_t mask) {
    uint16_t port = (pic_num == 1) ? PIC1_DATA : PIC2_DATA;
    outb(port, mask);
}

// MARK :- Helpers
void pic_send_eoi(uint8_t irq)
{
  if (irq >= 8) {
    outb(PIC2_COMMAND, PIC_EOI);
  }

  // Always signal PIC1 that an interrupt has been handled
  // because it's the PIC that forwards PIC2's irqs as well.
  outb(PIC1_COMMAND, PIC_EOI);
}

static uint16_t pic_get_irq_reg(int ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register */
static uint16_t pic_get_irr(void)
{
    return pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register */
static uint16_t pic_get_isr(void)
{
    return pic_get_irq_reg(PIC_READ_ISR);
}
