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
	// init serial port for logging
	init_serial();
	logv("Welcome to Polo! I'm a toy operating system. Right now, I don't do much, but I hope to be able to be a minimal OS shell when I grow up :)");
	logv("Loading GDT");
	load_gdt();
	logv("Done loading GDT");

	// Setup interrupts!
	// load_idt();

	// init the terminal
	logv("Initializing terminal");
	terminal_initialize();
}

void kernel_main(void)
{
	printf("Hello, World! Looks like I made it through the boot sequence alive!\n");
}
