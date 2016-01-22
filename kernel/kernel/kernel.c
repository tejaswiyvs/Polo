#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <serial.h>
#include <log.h>
#include <kernel/tty.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/ps2.h>
#include <kernel/keyboard.h>
#include <kernel/pit.h>

void kernel_early(void)
{
	// disable interrupts.
	asm volatile("cli");
}

void kernel_main(void)
{
	// init serial port for logging
	init_serial();
	terminal_initialize();

	printf("********************************************************************************");
	printf("*                                POLO                                          *");
	printf("*                                ----                                          *");
	printf("*                   Welcome to Polo! I'm a hobby OS!                           *");
	printf("*                                                                              *");
	printf("********************************************************************************");
	printf("\n");

	for (int i = 0; i < 100; i++) {
		printf("This is a line\n");
	}

	gdt_init();
	idt_init();
	ps2_init();
	pit_init();

	while(1) {
		putchar(getchar());
	}

	printf("Uhoh!\n Something went wrong we're not supposed to be here\n");
}
