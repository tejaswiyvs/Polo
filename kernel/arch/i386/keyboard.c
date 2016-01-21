#include <kernel/keyboard.h>
#include <kernel/ps2.h>
#include <kernel/isr.h>
#include <stdbool.h>
#include <stdio.h>
#include <asm.h>

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

// unsigned char scan_codes_kbd_us[128] =
// {
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\t', '`', 0,
//   0, 0, 0, 0, 0, 'q', '1', 0, 0, 'z', 's', 'a', 'w', '2', 0,
//   0, 'c', 'x', 'd', 'e', '4', '3', 0, 0, ' ', 'v', 'f', 't', 'r', '5', 0,
//   0, 'n', 'b', 'h', 'g', 'y', '6', 0, 0, 0, 'm', 'j', 'u', '7', '8', 0
// };

unsigned char scan_codes_kbd_us[128] =
{
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, '\t', '`', 0,
  0, 0, 0, 0,
  0, 'q', '1', 0,
  0, 0, 'z', 's',
  'a', 'w', '2', 0,
  0, 'c', 'x', 'd',
  'e', '4', '3', 0,
  0, ' ', 'v', 'f',
  't', 'r', '5', 0,
  0, 'n', 'b', 'h',
  'g', 'y', '6', 0,
  0, 0, 'm', 'j',
  'u', '7', '8', 0,
  0, ',', 'k', 'i',
  'o', '0', '9', 0,
  0, '.', '/', 'l',
  ';', 'p', '-', 0,
  '[', '=', 0, 0,
  0, 0, 0, ']',
  0, '\\', 0, 0,
  0, 0, 0, 0,
  0, '1', 0, '4',
  '7', 0, 0, 0,
  '0', '.', '2', '5',
  '6', '8', 0, 0,
  0, '+', '3', '-',
  '*', '9', 0, 0,
  0, 0, 0, 0
};

void keyboard_init(uint8_t port_id)
{
	printf("Init keyboard\n");
  keyboard_port_id = port_id;

	// Setup scan code set 2.
	ps2_send_cmd(keyboard_port_id, CMD_RW_SCAN_CODE_SET);
	if (!ps2_send_cmd_ack(keyboard_port_id, 2, 3)) printf("Failed to set scan code set\n");
  ps2_poll_data();
  register_interrupt_handler(33, &keyboard_int_handler);
}

void keyboard_int_handler(register_t regs)
{
  putchar(getchar());
}

char get_scan_code()
{
  char c = 0;
  do {
    if(ps2_read_data() != c) {
      c = ps2_read_data();
      if(c > 0) return c;
    }
  } while(1);
  return;
}

char getchar()
{
  return scan_codes_kbd_us[get_scan_code()];
}

void empty_keyboard_buffer()
{
	keyboard_buffer_idx = 0;
	for (int i = 0; i < KBD_BUFFER_SIZE; i++) {
		keyboard_buffer[i] = 0;
	}
}
