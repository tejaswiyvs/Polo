#include "../include/kernel/idt.h"
#include <asm.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <log.h>

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
  uint16_t base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
  uint16_t sel; // Kernel segment selector.
  uint8_t always;      // This must always be zero.
  uint8_t flags; // More flags. See documentation.
  uint16_t base_hi; // The upper 16 bits of the address to jump to.
} __attribute__((packed));

struct IDTPtr {
  uint16_t limit;
  uint16_t base;
} __attribute__((packed));

typedef struct IDTDescr idt_entry_t;
typedef struct IDTPtr idt_ptr_t;

// Yikes! Address of each ISR. ISRs in assembly
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void idt_flush();

static void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

void load_idt() {
  idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
  idt_ptr.base = (uint32_t) &idt_entries;

  // We've allocated space for 256 entries, but we only care about 31 of them
  // for now. As long as we provide a null interrupt handler to the CPU
  // It's satisfied, but we *have* to provide one for each interrupt from 0 - 256
  memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

  idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
  idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
  idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
  idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
  idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
  idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
  idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
  idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
  idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
  idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
  idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
  idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
  idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
  idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
  idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
  idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
  idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
  idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
  idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
  idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
  idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
  idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
  idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
  idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
  idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
  idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
  idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
  idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
  idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
  idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
  idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

  logv("Flushing IDT");
  idt_flush((uint32_t)&idt_ptr);
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
  idt_entries[num].base_lo = base & 0xFFFF;
  idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
  idt_entries[num].sel = selector;
  idt_entries[num].always = 0;

  // We must uncomment the OR below when we get to the user-mode.
  // It sets the interrupt gate's privilege level to 3.
  idt_entries[num].flags = flags /* | 0x60 */;
}
