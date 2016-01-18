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
int ps2_poll_data();

void ps2_send_cmd(uint8_t channel_id, uint8_t cmd);
uint8_t ps2_read_data();
void ps2_send_p1(uint8_t cmd);
void ps2_send_p2(uint8_t cmd);
uint8_t ps2_read_p1();
uint8_t ps2_read_p2();
void dummy_read();
/*
 * Sends a byte to the selected port. If there's a response, it will be retured.
 * If the operation is just to send and no response is expected, a 0 will be returned.
 */
uint8_t ps2_send_byte(uint8_t port_id, uint8_t byte);

#endif
