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
}

void kernel_main(void)
{
	// init serial port for logging
	init_serial();

	logv("Welcome to Polo! I'm a toy operating system. Right now, I don't do much, but I hope to be able to be a minimal OS shell when I grow up :)");

	// Setup the GDT
	logv("Loading GDT");
	load_gdt();
	logv("Done loading GDT");

	// Setup interrupts!
	logv("Loading interrupts");
	load_idt();
	logv("Done loading IDT");

	// init the terminal
	logv("Initializing terminal");
	terminal_initialize();

	printf("********************************************************************************");
	printf("*                                POLO                                          *");
	printf("*                                ----                                          *");
	printf("*                                                                              *");
	printf("*      Welcome to Polo! I'm a hobby OS! I was built by Tejaswi Yerukalapudi    *");
	printf("*                                                                              *");
	printf("*                                                                              *");
	printf("********************************************************************************");

	shell();

	while(1);
}
