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

	uint8_t hex_value = 0xAA;
	printf("test hex printing: %X", hex_value);


	gdt_init();
	idt_init();
	ps2_init();
	timer_init();

	while(1) {
		asm ("hlt");
	}
	// while(1) {
	// 	putchar(getchar());
	// }
}
