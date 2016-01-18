#include <kernel/ps2.h>
#include <asm.h>
#include <stdbool.h>
#include <stdio.h>

#define PS2_DATA    0x60
#define PS2_CMD     0x64
#define PS2_STATUS  0x64

#define CMD_TEST_PS2                0xAA
#define CMD_READ_CONFIGURATION_BYTE   0x20
#define CMD_WRITE_CONFIGURATION_BYTE  0x60
#define CMD_TEST_FIRST_PS2          0xAB
#define CMD_DISABLE_FIRST_PS2       0xAD
#define CMD_ENABLE_FIRST_PS2        0xAE
#define CMD_TEST_SECOND_PS2         0xA9
#define CMD_ENABLE_SECOND_PS2       0xA8
#define CMD_DISABLE_SECOND_PS2      0xA7
#define CMD_DEVICE_RESET          0xFF
#define CMD_SEND_P2               0xD4

static bool ps2_channel1_available = true;
static bool ps2_channel2_available = true;
static bool ps2_channel1_connected = false;
static bool ps2_channel2_connected = false;

static void flush_ps2_controller();
static bool configure_controller();
static void enable_ps2_channels();
static void disable_ps2_channels();
static void reset_and_test_devices();
static uint8_t get_configuration_byte();
static void set_configuration_byte(uint8_t config_byte_value);
static void dummy_read();
static int reset_channel(uint8_t channel_id);

void ps2_init()
{
  // TODO:
  // disable_usb_controller()
  // We have to check if PS2 exists at all
  // but since we only care about bochs right now, it's okay to skip that bit.

  disable_ps2_channels();
  flush_ps2_controller();

  // Try to detect and configure the controller.
  if (!configure_controller()) {
    return;
  }

  reset_and_test_devices();
}

static void disable_ps2_channels()
{
  outb(PS2_CMD, CMD_DISABLE_FIRST_PS2);

  // This command will be ignored if the PS2 Controller is a single channel device
  outb(PS2_CMD, CMD_DISABLE_SECOND_PS2);
}

static void enable_ps2_channels()
{
  if (ps2_channel1_available) {
    outb(PS2_CMD, CMD_ENABLE_FIRST_PS2);
  }

  if (ps2_channel2_available) {
    outb(PS2_CMD, CMD_ENABLE_SECOND_PS2);
  }
}

// Reads from data buffer and discards data.
static void flush_ps2_controller()
{
  inb(PS2_DATA);
}

// Some controller configuration bytes can be invalid. Reconfigure with right
// Configuration.
static bool configure_controller()
{
  // We want to zero out bits 0, 1, 6 (disable interrupts while we're still setting up)
  // and clear the port translation stuff in the config_byte.
  // http://wiki.osdev.org/%228042%22_PS/2_Controller
  // See: PS/2 Controller configuration byte
  uint8_t config_byte = get_configuration_byte();
  config_byte &= ~(1 << 0);
  config_byte &= ~(1 << 1);
  config_byte &= ~(1 << 6);
  set_configuration_byte(config_byte);

  outb(PS2_CMD, CMD_TEST_PS2);
  uint8_t self_test = inb(PS2_DATA);
  if (self_test == 0x55) {
    printf("PS2 Controller succesfully initialized\n");
  }
  else {
    printf("PS2 controller not found\n");
    return false;
  }

  outb(PS2_CMD, CMD_TEST_FIRST_PS2);
  uint8_t response = inb(PS2_DATA);
  if (response == 0) {
    ps2_channel1_available = true;
  }

  outb(PS2_CMD, CMD_TEST_SECOND_PS2);
  response = inb(PS2_DATA);
  if (response == 0) {
    ps2_channel2_available = true;
  }

  if (!ps2_channel1_available && !ps2_channel2_available) {
    printf("No PS/2 Peripherals found\n");
    return false;
  }

  enable_ps2_channels();

  printf("Detected %d channels available on PS2 controller\n", ps2_channel1_available + ps2_channel2_available);
  return true;
}

static void reset_and_test_devices()
{
  uint8_t configuration_byte = get_configuration_byte();

  if (ps2_channel1_available) {
    bool result = reset_channel(1);
    if (result) {
      configuration_byte |= 1;
    }
  }
  if (ps2_channel2_available) {
    bool result = reset_channel(2);
    if (result) {
      configuration_byte |= 0x2;
    }
  }

  asm ("xchgw %bx, %bx");
  set_configuration_byte(configuration_byte);
}

static int reset_channel(uint8_t channel_id)
{
  if (channel_id != 1 && channel_id != 2) { return false; }

  if (channel_id == 1) {
    ps2_send_p1(CMD_DEVICE_RESET);
  }
  if (channel_id == 2) {
    ps2_send_p2(CMD_DEVICE_RESET);
  }

  bool connected = false;
  uint8_t data;

  if (ps2_poll_in() == 0) {
    printf("No peripheral connected to PS/2 Port %d\n", channel_id);
  }
  else {
    data = inb(PS2_DATA);
    if (data == 0xFA) {
      printf("PS2: Port %d succesfully initialized\n", channel_id);
      ps2_poll_in();
      printf("PS2: Port %d, DeviceType: %x\n", channel_id, inb(PS2_DATA));
    }
    else {
      printf("PS2: Port %d Reset failed\n", channel_id);
      connected = false;
    }
  }

  if (channel_id == 1) {
    ps2_channel1_connected = connected;
  }
  else if (channel_id == 2) {
    ps2_channel2_connected = connected;
  }

  return connected;
}

static uint8_t get_configuration_byte()
{
  outb(PS2_CMD, CMD_READ_CONFIGURATION_BYTE);
  return inb(PS2_DATA);
}

static void set_configuration_byte(uint8_t config_byte_value)
{
  outb(PS2_CMD, CMD_WRITE_CONFIGURATION_BYTE);
  outb(PS2_DATA, config_byte_value);
}

uint8_t ps2_send_byte(uint8_t port_id, uint8_t byte)
{
  if (port_id == 1 && !ps2_channel1_available) { return 0; }
  if (port_id == 2 && !ps2_channel2_available) { return 0; }

  return 0;
}

// Polls till the current op is complete by the PS2 controller
static void dummy_read()
{
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
}

int ps2_poll_in()
{
    for (uint32_t i = 0; i < 0x5000000; i++) {
        uint8_t stat = inb(PS2_STATUS);
        if ((stat & 0b00000001) != 0) {
            return 1;
        }
    }
    return 0;
}

int ps2_poll_out()
{
    for (uint32_t i = 0; i < 0x5000000; i++) {
        uint8_t stat = inb(PS2_STATUS);
        if ((stat & 0b00000010) == 0) {
            return 1;
        }
    }
    return 0;
}

void ps2_send_p1(uint8_t cmd)
{
    ps2_poll_out();
    outb(PS2_DATA, cmd);
}

void ps2_send_p2(uint8_t cmd)
{
    outb(PS2_CMD, CMD_SEND_P2);
    ps2_poll_out();
    outb(PS2_DATA, cmd);
}
