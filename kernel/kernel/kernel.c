#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <serial.h>
#include <log.h>

#include <kernel/tty.h>
#include "../include/kernel/idt.h"
#include "../include/kernel/gdt.h"

void kernel_early(void)
{
	// disable interrupts.
	asm volatile("cli");
	load_gdt();
}

void kernel_main(void)
{
	// init serial port for logging
	init_serial();

	logv("Welcome to Polo! I'm a toy operating system. Right now, I don't do much, but I hope to be able to be a minimal OS shell when I grow up :)");

	// Setup the GDT
	// Interrupts will be disabled by load_gdt()
	logv("Loading GDT");
	logv("Done loading GDT");

	// Setup interrupts!
	logv("Loading interrupts");
	load_idt();
	logv("Done loading IDT");

	// Trigger some interrupts to ensure they're working
	asm volatile ("xchgw %bx, %bx");
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
	asm volatile ("xchgw %bx, %bx");

	// init the terminal
	logv("Initializing terminal");
	terminal_initialize();
	printf("Hello, World! Looks like I made it through the boot sequence alive!\n");
}
