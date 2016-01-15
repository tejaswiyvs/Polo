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
	load_gdt();
	// do_test();
	load_idt();

	// init serial port for logging
	init_serial();
	terminal_initialize();

	printf("********************************************************************************");
	printf("*                                POLO                                          *");
	printf("*                                ----                                          *");
	printf("*                                                                              *");
	printf("*      Welcome to Polo! I'm a hobby OS! I was built by Tejaswi Yerukalapudi    *");
	printf("*                                                                              *");
	printf("*                                                                              *");
	printf("********************************************************************************");

	// Test an interrupt
	asm volatile ("xchgw %bx, %bx");

	// int i = 0;
	// for (i = 0; i < 10; i++) {
	asm volatile ("int $0x3");
	// asm volatile ("int $0x3");
	// }

	while(1);
}
