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

void keyboard_int_handler(register_t regs);
char get_scan_code();
char get_char();

static char scode[] =
{
	0x04, 0x08, 0x0C, 0x10
};

void keyboard_init(uint8_t port_id)
{
	printf("Init keyboard\n");
  keyboard_port_id = port_id;


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
  uint8_t scan_code = 0;
	while ((scan_code = ps2_read_data()) != PS2_TIMEOUT) { printf("%x", scan_code); }

	// if ((scan_code = ps2_read_data()) == PS2_TIMEOUT) return 0x0;
	// printf("Scan code: %x\n", scan_code);
	return scan_code;
}

char get_char()
{
  return scode[get_scan_code() + 1];
}

void process()
{
}
