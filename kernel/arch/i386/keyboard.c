#include <kernel/keyboard.h>
#include <kernel/ps2.h>
#include <kernel/isr.h>
#include <stdbool.h>

uint8_t keyboard_port_id;

void process();

#define CMD_CHANGE_LED_STATES 0xED
#define CMD_ECHO              0xEE
#define CMD_RW_SCAN_CODE_SET  0xF0
#define CMD_SET_TYPEMATIC     0xF3
#define CMD_SET_DEFAULTS      0xF6
#define RESPONSE_KBD_ACK      0xFA
#define RESPOSNE_KBD_RESEND   0xFE
#define KBD_BUFFER_SIZE				255

void keyboard_int_handler(register_t regs);
char get_scan_code();
char get_char();
void empty_keyboard_buffer();

char keyboard_buffer[KBD_BUFFER_SIZE];
int keyboard_buffer_idx = 0;

unsigned char scan_codes_kbd_us[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

void keyboard_init(uint8_t port_id)
{
	printf("Init keyboard\n");
  keyboard_port_id = port_id;

	// Setup scan code set 2.
	ps2_send_cmd(keyboard_port_id, CMD_RW_SCAN_CODE_SET);
	if (!ps2_send_cmd_ack(keyboard_port_id, 2, 3)) printf("Failed to set scan code set\n");
  register_interrupt_handler(33, &keyboard_int_handler);
}

void keyboard_int_handler(register_t regs)
{
  get_scan_code();
}

char get_scan_code()
{
	uint8_t scan_code = ps2_read_data();
	if (scan_code == PS2_TIMEOUT) return;

	if (scan_code & 0x80) {
		// Key was released
		// Ignore this for now.
	}

	if (keyboard_buffer_idx == (KBD_BUFFER_SIZE - 1)) {
		empty_keyboard_buffer();
	}

	keyboard_buffer[keyboard_buffer_idx] = scan_codes_kbd_us[scan_code];
	return;
}

char getchar()
{
	while(keyboard_buffer_idx == 0);
	char ch = keyboard_buffer[keyboard_buffer_idx];
	keyboard_buffer_idx--;
	return ch;
}

void empty_keyboard_buffer()
{
	keyboard_buffer_idx = 0;
	for (int i = 0; i < KBD_BUFFER_SIZE; i++) {
		keyboard_buffer[i] = 0;
	}
}
