#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include "../include/kernel/idt.h"

void kernel_early(void)
{
	terminal_initialize();
	load_idt();
}

void kernel_main(void)
{
	printf("Hello, kernel World!\n");
}
