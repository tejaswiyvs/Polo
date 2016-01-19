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
#define CMD_IDENTIFY              0xF2
#define CMD_DISABLE_SCANNING      0xF5
#define CMD_ENABLE_SCANNING       0xF4

#define RESPONSE_SELF_TEST_PASSED 0xAA

static bool ps2_port1_available = true;
static bool ps2_port2_available = true;
static bool ps2_port1_connected = false;
static bool ps2_port2_connected = false;

typedef enum PS2DeviceType {
  Keyboard,
  Mouse,
  Unknown,
  NotConnected
} ps2_device_type_t;

static bool configure_controller();
static void enable_ps2_ports();
static void disable_ps2_ports();
static void reset_and_test_devices();
static uint8_t get_configuration_byte();
static void set_configuration_byte(uint8_t config_byte_value);
static int reset_port(uint8_t port_id);
static ps2_device_type_t init_and_get_device_type(uint8_t port_id, void (*ptr)(uint8_t));
void ps2_send_p1(uint8_t cmd);
void ps2_send_p2(uint8_t cmd);
uint8_t ps2_read_p1();
uint8_t ps2_read_p2();

// Public API
void ps2_init()
{
  // TODO:
  // disable_usb_controller()
  // We have to check if PS2 exists at all
  // but since we only care about bochs right now, it's okay to skip that bit.

  disable_ps2_ports();
  flush_ps2_controller();

  // Try to detect and configure the controller.
  if (!configure_controller()) {
    return;
  }

  reset_and_test_devices();
}

void ps2_send_cmd(uint8_t port_id, uint8_t cmd)
{
  if (port_id != 1 && port_id != 2) { return CMD_ERR; }
  if (port_id == 1 && !ps2_port1_connected) { return CMD_ERR; }
  if (port_id == 2 && !ps2_port2_connected) { return CMD_ERR; }

  if(!ps2_poll_out()) return PS2_TIMEOUT;

  if (port_id == 1) {
    ps2_send_p1(cmd);
  }
  else {
    ps2_send_p2(cmd);
  }
}

// Reads from data buffer and discards data.
void flush_ps2_controller()
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

uint8_t ps2_read_data()
{
  if (!ps2_poll_in()) return PS2_TIMEOUT;
  return inb(PS2_DATA);
}

static void disable_ps2_ports()
{
  outb(PS2_CMD, CMD_DISABLE_FIRST_PS2);
  outb(PS2_CMD, CMD_DISABLE_SECOND_PS2);
}

static void enable_ps2_ports()
{
  if (ps2_port1_available) {
    outb(PS2_CMD, CMD_ENABLE_FIRST_PS2);
  }

  if (ps2_port2_available) {
    outb(PS2_CMD, CMD_ENABLE_SECOND_PS2);
  }
}

// Private methods

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
    ps2_port1_available = true;
  }

  outb(PS2_CMD, CMD_TEST_SECOND_PS2);
  response = inb(PS2_DATA);
  if (response == 0) {
    ps2_port2_available = true;
  }

  if (!ps2_port1_available && !ps2_port2_available) {
    printf("No PS/2 Peripherals found\n");
    return false;
  }

  enable_ps2_ports();

  printf("Detected %d ports available on PS2 controller\n", ps2_port1_available + ps2_port2_available);
  return true;
}

static void reset_and_test_devices()
{
  uint8_t configuration_byte = get_configuration_byte();

  if (ps2_port1_available) {
    bool result = reset_port(1);
    if (result) {
      configuration_byte |= 1;
    }
  }
  if (ps2_port2_available) {
    bool result = reset_port(2);
    if (result) {
      configuration_byte |= 0x2;
    }
  }

  set_configuration_byte(configuration_byte);
}

static int reset_port(uint8_t port_id)
{
  if (port_id != 1 && port_id != 2) { return false; }
  void (*ptr)(uint8_t cmd);
  if(port_id == 1) {
    ptr = &ps2_send_p1;
  }
  else {
    ptr = &ps2_send_p2;
  }

  ptr(CMD_DEVICE_RESET);
  uint8_t data;

  if (ps2_poll_in() == 0) {
    printf("No peripheral connected to PS/2 Port %d\n", port_id);
  }
  else {
    data = inb(PS2_DATA);
    if (data == PS2_ACK) {
      // The command to the PS2 device was acknowledge. Read the data for status
      if (ps2_poll_in() == 0) return false;
      uint8_t status = inb(PS2_DATA);
      if (status == RESPONSE_SELF_TEST_PASSED) {
        printf("PS2: Port %d succesfully initialized\n", port_id);
        uint8_t device_type = init_and_get_device_type(port_id, ptr);
        if (device_type == NotConnected) {
          printf("There was an error initializing the PS2 Device Connected on port: %d\n", port_id);
          return false;
        }
        else if (device_type == Keyboard) {
          mark_connected(port_id);
          keyboard_init(port_id);
          ptr(CMD_ENABLE_SCANNING);
          return true;
        }
        else if (device_type == Mouse) {
          mark_connected(port_id);
          ptr(CMD_ENABLE_SCANNING);
          return true;
        }
      }
      else {
        printf("PS2: Port %d self test failure; Device respoded with: %x\n", port_id, status);
      }
    }
    else {
      printf("PS2: Port %d Reset failed. Device did not respond with an ACK\n", port_id);
    }
  }

  return false;
}

static ps2_device_type_t init_and_get_device_type(uint8_t port_id, void (*ptr)(uint8_t))
{
  ptr(CMD_DISABLE_SCANNING);
  ps2_poll_in();
  uint8_t status = inb(PS2_DATA);
  if (status == PS2_ACK)
  {
    ptr(CMD_IDENTIFY);
    if (!ps2_poll_in()) { return NotConnected; }
    status = inb(PS2_DATA);
    if (status == PS2_ACK)
    {
      // Device acknowledged the command, but timed out before
      // it responded. This might mean there's an old AT Keyboard connected
      // if we're currently polling port 1.
      if (!ps2_poll_in()) {
        if (port_id == 1) {
          printf("Detected Ancient AT Keyboard with translation enabled\n");
          return Keyboard;
        }
        else {
          return NotConnected;
        }
      }

      // Device may return one or more bytes for status.
      uint8_t byte1 = inb(PS2_DATA);
      uint8_t byte2 = 0;
      if (ps2_poll_in()) {
        byte2 = inb(PS2_DATA);
      }

      if (byte2 != 0) {
        if ((byte1 == 0xAB && byte2 == 0x41) || (byte1 == 0xAB && byte2 == 0xC1)) {
          printf("Detected MF2 Keyboard with translation enabled connected to PS2: port %d\n", port_id);
          return Keyboard;
        }
        else if (byte1 == 0xAB && byte2 == 0x83) {
          printf("Detected MF2 Keyboard connceted to PS/2 Port %d\n", port_id);
          return Keyboard;
        }
      }
      else {
        if (byte1 == 0x00) {
          printf("Detected PS/2 Mouse\n");
          return Mouse;
        }
        else if (byte1 == 0x03) {
          printf("Detected PS/2 Mouse with scroll-wheel\n");
          return Mouse;
        }
        else if (byte1 == 0x04) {
          printf("Detected 5 button Mouse.\n");
          return Mouse;
        }
      }
    }
  }

  return NotConnected;
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

// Polls till it's safe to read from the PS2 controller's data buffer.
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

// Polls till it's safe to write to the PS2 Data buffer.
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
    if (!ps2_poll_out()) printf("Cmd: %x timed out.\n", cmd);
    outb(PS2_DATA, cmd);
}

void ps2_send_p2(uint8_t cmd)
{
    if (!ps2_poll_out()) printf("Cmd: %x timed out.\n", CMD_SEND_P2);
    outb(PS2_CMD, CMD_SEND_P2);
    if (!ps2_poll_out()) printf("Cmd: %x timed out.\n", cmd);
    outb(PS2_DATA, cmd);
}

void mark_connected(port_id)
{
  if (port_id == 1) {
    ps2_port1_connected = true;
  }
  else {
    ps2_port2_connected = true;
  }
}
