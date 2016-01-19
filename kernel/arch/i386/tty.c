#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/vga.h>
#include <asm.h>
#include <log.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

static void wrap_around_if_needed();
static void buffer_wrap_around();

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_WHITE, COLOR_BLUE);
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		terminal_row++;
		terminal_column = 0;
		wrap_around_if_needed();
		return;
	}
	else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		terminal_column++;
		wrap_around_if_needed();
	}
}

static void wrap_around_if_needed()
{
	if (terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		terminal_row++;
	}

	if (terminal_row == VGA_HEIGHT)
	{
		buffer_wrap_around();
		terminal_row = VGA_HEIGHT - 1;
		terminal_row = 0;
	}

	terminal_set_cursor(terminal_row, terminal_column);
}

static void buffer_wrap_around()
{
	uint16_t buffer_copy[VGA_WIDTH * VGA_HEIGHT];
	// for (size_t row = 1; row < VGA_HEIGHT; row++) {
	// 	for (size_t col = 0; col < VGA_WIDTH; col++) {
	// 		buffer_copy[(row - 1) * VGA_HEIGHT + col] = terminal_buffer[row * VGA_WIDTH + col];
	// 	}
	// }
	memcpy(buffer_copy, terminal_buffer[VGA_WIDTH], (VGA_WIDTH - 1) * VGA_HEIGHT * sizeof(uint16_t));
	// Set the terminal color properly for the last row.
	for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
		buffer_copy[i] = make_vgaentry(' ', terminal_color);
	}
	memcpy(terminal_buffer, buffer_copy, VGA_WIDTH * VGA_HEIGHT * sizeof(uint16_t));
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_set_cursor(int row, int col)
{
	 unsigned short position=(row*80) + col;

	 // cursor LOW port to vga INDEX register
	 outb(0x3D4, 0x0F);
	 outb(0x3D5, (unsigned char)(position&0xFF));
	 // cursor HIGH port to vga INDEX register
	 outb(0x3D4, 0x0E);
	 outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}
