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
bool set_keyboard_scan_code_set(uint8_t scan_code_set, uint8_t retries);

static char scode[] =
{
	0x04, 0x08, 0x0C, 0x10
};

void keyboard_init(uint8_t port_id)
{
  keyboard_port_id = port_id;
	if (!set_keyboard_scan_code_set(2, 3)) {
		printf("Error initalizing keyboard.\n");
		return;
	}
	printf("Scan code set 2 done.\n");
  register_interrupt_handler(33, &keyboard_int_handler);
}

void keyboard_int_handler(register_t regs)
{
  get_scan_code();
}

bool set_keyboard_scan_code_set(uint8_t scan_code_set, uint8_t retries)
{
	if (retries == 0) { return false; }

	ps2_send_cmd(keyboard_port_id, CMD_RW_SCAN_CODE_SET);
	ps2_send_cmd(keyboard_port_id, 2);
	uint8_t response;
	if ((response = ps2_read_data()) == PS2_TIMEOUT) { return false; }

	if (response == PS2_RESEND) {
		set_keyboard_scan_code_set(scan_code_set, retries - 1);
	}
	else if (response == PS2_TIMEOUT) {
		return false;
	}
	else if (response == PS2_ACK) {
		return true;
	}

	return false;
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
