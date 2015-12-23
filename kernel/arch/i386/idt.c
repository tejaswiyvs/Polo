#include "../include/kernel/idt.h"
#include <asm.h>
#include <stdio.h>
#include <stdint.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIC1_OFFSET 0x70
#define PIC2_OFFSET 0x78

struct IDTDescr {
  uint16_t offset_1; // The lower 16 bits of the address to jump to when this interrupt fires.
  uint16_t selector; // Kernel segment selector.
  uint8_t zero;      // This must always be zero.
  uint8_t type_attr; // More flags. See documentation.
  uint16_t offset_2; // The upper 16 bits of the address to jump to.
};

typedef struct IDTDescr idt_entry_t;

void IRQ_set_mask(unsigned char IRQline);
void IRQ_clear_mask(unsigned char IRQline);
void PIC_sendEOI(unsigned char irq);
static inline void lidt(void* base, uint16_t size);

 // Why:
 // The CPU normally reserves INT 0-31 for it's private use to notify
 // catastrophic failures. However, the master PIC is mapped to 08h to 0Fh.
 // this obviously causes a conflict, how do we know if an interrupt (say INT9) is
 // rom a catastrophic OS error, or if it's because of a hardware interrupt? We
 // can probe the hardware to check if an interrupt did occur, and take action
 // based on that, but that's slow, and hacky. So insted, we remap the PIC
 // interrupts from 08h-0Fh to a starting location that's a multiple of 08h
 // In this case, we pick PIC2 = 070h-077h, PIC1= 078h-7Fh
 //
 // Note that IRQs can only be mapped to INTs that are multiples of 08h: 00h-07h,
 // * 08h-0Fh, 10h-17h, 17h-1Fh.
void setup_and_remap_pics() {
  unsigned char a1, a2;

	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, PIC1_OFFSET);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, PIC2_OFFSET);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}

/* interrupts are handled by priority level: 0, 1, 2, 8, 9, 10, 11, 12, 13,
   14, 15, 3, 4, 5, 6, 7. */
void load_idt() {
  // This should initialize the PIC and remap it PIC1 to 0x70-07x77 and
  // PIC2 to 0x78-0x7E
  setup_and_remap_pics();

  // We want to now setup and load the IDT. We only care about keyboard
  // interrupts now, so just ack everything else blindly.

}

void PIC_sendEOI(unsigned char irq) {
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);

	outb(PIC1_COMMAND,PIC_EOI);
}

static inline void lidt(void* base, uint16_t size)
{
    struct
    {
        uint16_t length;
        uint32_t base;
    } __attribute__((packed)) IDTR;

    IDTR.length = size;
    IDTR.base = (uint32_t) base;
    asm ( "lidt (%0)" : : "p"(&IDTR) );
}

//
// void IRQ_set_mask(uint8_t IRQline) {
//     uint16_t port;
//     uint8_t value;
//
//     if(IRQline < 8) {
//         port = PIC1_DATA;
//     } else {
//         port = PIC2_DATA;
//         IRQline -= 8;
//     }
//     value = inb(port) | (1 << IRQline);
//     outb(port, value);
// }

// void IRQ_clear_mask(unsigned char IRQline) {
//     uint16_t port;
//     uint8_t value;
//
//     if(IRQline < 8) {
//         port = PIC1_DATA;
//     } else {
//         port = PIC2_DATA;
//         IRQline -= 8;
//     }
//     value = inb(port) & ~(1 << IRQline);
//     outb(port, value);
// }
//
