#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include "../include/kernel/idt.h"
#include "../include/kernel/gdt.h"

void kernel_early(void)
{
	// GRUB does set these up for us,
	// but we want to be safe and write exactly what we want.
	load_gdt();
	load_idt();

	// init the terminal
	terminal_initialize();
}

void kernel_main(void)
{
	printf("Hello, kernel World!\n");
}
