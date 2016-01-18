#include <kernel/keyboard.h>
#include <kernel/ps2.h>
#include <kernel/isr.h>

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
uint8_t set_keyboard_scan_code_set(uint8_t scan_code_set, uint8_t retries);

static char scode[] =
{
	'0', '\0', '0', '\0', 0x1b, '\0',
	'1', '\0', '2', '\0', '3', '\0', '4', '\0', '5', '\0', '6', '\0', '7', '\0', '8', '\0', '9', '\0', '0', '\0', '-', '\0', '=', '\0', 0x8, '\0',
	0x9, '\0', 'q', '\0', 'w', '\0', 'e', '\0', 'r', '\0', 't', '\0', 'y', '\0', 'u', '\0', 'i', '\0', 'o', '\0', 'p', '\0', '[', '\0', ']', '\0',
	0xA, '\0', 0, '\0', 'a', '\0', 's', '\0', 'd', '\0', 'f', '\0', 'g', '\0', 'h', '\0', 'j', '\0', 'k', '\0', 'l', '\0', ';', '\0', '\'', '\0',
	'`', '\0', 0x0, '\0', '\\', '\0', 'z', '\0', 'x', '\0', 'c', '\0', 'v', '\0', 'b', '\0', 'n', '\0', 'm', '\0', ',', '\0', '.', '\0', '/', '\0',
	0x0, '\0', '*', '\0', 0x0, '\0', ' ', '\0', 0x0, '\0', 0x0, '\0'
};

void keyboard_init(uint8_t port_id)
{
  keyboard_port_id = port_id;
	dummy_read();
	if (set_keyboard_scan_code_set(2, 3) != CMD_DONE) {
		printf("Error initalizing keyboard.\n");
		return;
	}
	else {
		printf("Scan code set 2 done.");
	}

  register_interrupt_handler(33, &keyboard_int_handler);
}

void keyboard_int_handler(register_t regs)
{
  char ch = get_char();
  putchar(ch);
}

uint8_t set_keyboard_scan_code_set(uint8_t scan_code_set, uint8_t retries)
{
	if (retries == 0) { return CMD_ERR; }

	ps2_send_cmd(keyboard_port_id, CMD_RW_SCAN_CODE_SET);
	ps2_send_cmd(keyboard_port_id, 2);
	asm ("xchgw %bx, %bx");
	uint8_t response = ps2_read_data();
	printf("Response after setting scan code = %x", response);
	if (response == PS2_RESEND) {
		set_keyboard_scan_code_set(scan_code_set, retries - 1);
	}
	else if (response == PS2_TIMEOUT) {
		return CMD_ERR;
	}
	else if (response == PS2_ACK) {
		return CMD_DONE;
	}

	uint8_t new_set = ps2_read_data();
	if (new_set != scan_code_set) {
		printf("Something went wrong setting scan codes\n. Current set = %x", new_set);
	}

	return CMD_ERR;
}

char get_scan_code()
{
  uint8_t scan_code = 0;
  for (int i = 0; i < 500; i++) {
    scan_code = ps2_read_data();
		printf("%d", scan_code);
    if (scan_code > 0) return scan_code;
  }
  return 0x0;
}

char get_char()
{
  return scode[get_scan_code() + 1];
}

void process()
{
}
