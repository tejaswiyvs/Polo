#ifndef __ps2_h
#define __ps2_h

#include <stdint.h>

#define CMD_ERR     0
#define CMD_DONE    1
#define PS2_ACK     0xFA
#define PS2_RESEND  0xFE
#define PS2_TIMEOUT 0
#define PS2_SENT    0x1

void ps2_init();

int ps2_poll_in();
int ps2_poll_out();
uint8_t ps2_read_data();
void ps2_send_cmd(uint8_t channel_id, uint8_t cmd);
void flush_ps2_controller();

#endif
