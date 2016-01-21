#ifndef __ps2_h
#define __ps2_h

#include <stdint.h>
#include <stdbool.h>

#define CMD_ERR     0
#define CMD_DONE    1
#define PS2_ACK     0xFA
#define PS2_RESEND  0xFE
#define PS2_TIMEOUT 0
#define PS2_SENT    0x1

void ps2_init();

bool ps2_poll_in();
bool ps2_poll_out();

// Polls the ISR for bit 2 to be toggled before performing the read. Can timeout
uint8_t ps2_poll_data();

// Returns whatever is in the data buffer.
uint8_t ps2_read_data();

uint8_t ps2_send_cmd(uint8_t port_id, uint8_t cmd);
bool ps2_send_cmd_ack(uint8_t port_id, uint8_t cmd, uint8_t num_retries);

#endif
