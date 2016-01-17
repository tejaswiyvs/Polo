#ifndef __ps2_h
#define __ps2_h

#include <stdint.h>

void ps2_init();
void ps2_poll();

/*
 * Sends a byte to the selected port. If there's a response, it will be retured.
 * If the operation is just to send and no response is expected, a 0 will be returned.
 */
uint8_t ps2_send_byte(uint8_t port_id, uint8_t byte);

#endif
