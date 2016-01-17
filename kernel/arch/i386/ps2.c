#include <kernel/ps2.h>
#include <asm.h>
#include <stdbool.h>
#include <stdio.h>

#define PS2_DATA    0x60
#define PS2_CMD     0x64

#define TEST_PS2                0xAA
#define PS2_CONFIGURATION_BYTE  0x20

#define TEST_FIRST_PS2          0xAB
#define DISABLE_FIRST_PS2       0xAD
#define ENABLE_FIRST_PS2        0xAE

#define TEST_SECOND_PS2         0xA9
#define ENABLE_SECOND_PS2       0xA8
#define DISABLE_SECOND_PS2      0xA7

#define READ_CONTROLLER_OUTPUT  0xD0

static bool ps2_channel1_available = true;
static bool ps2_channel2_available = true;

static void disable_ps2_controller();
static void flush_ps2_controller();
static bool configure_controller();

void ps2_init()
{
  // TODO:
  // disable_usb_controller()
  // We have to check if PS2 exists at all
  // but since we only care about bochs right now, it's okay to skip that bit.

  disable_ps2_controller();
  flush_ps2_controller();
  configure_controller();
}

static void disable_ps2_controller()
{
  asm ("xchgw %bx, %bx");
  outb(PS2_CMD, DISABLE_FIRST_PS2);

  // This command will be ignored if the PS2 Controller is a single channel device
  outb(PS2_CMD, DISABLE_SECOND_PS2);
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
  // Some bits of the config byte start in an "unknown" state. Reset these
  // to values we want. Specifically:
  //           0 : First PS2 Port Interrupt Enabled / disabled (1 = enabled, 0 = disabled)
  //           1 : Second PS2 Port Interrupt Enabled / disabled (1 = enabled, 0 = disabled)
  //           2 : System Flag (1 = system passed POST, 0 = your os shouldn't be running)
  //           3 : Zero
  //           4 : First PS2 Enabled / disabled (1 = disabled, 0 = enabled)
  //           5 : Second PS2 Enabled / disabled (1 = disabled, 0 = enabled)
  //           6 : First PS2 Port translation enabled / disabled (1 = enabled, 0 = disabled)
  //           7 : Zero
  outb(PS2_CMD, PS2_CONFIGURATION_BYTE);
  uint8_t config_byte = inb(PS2_DATA);
  config_byte &= ~(1 << 0);
  config_byte &= ~(1 << 1);
  config_byte &= ~(1 << 6);

  outb(PS2_CMD, TEST_PS2);
  uint8_t self_test = inb(PS2_DATA);
  if (self_test == 0x55) {
    printf("PS2 Controller succesfully initialized\n");
  }
  else {
    printf("PS2 Controller not found or uninitialized. PS2 Peripherals might not work.\n");
    return false;
  }

  asm ("xchgw %bx, %bx");
  outb(PS2_CMD, ENABLE_FIRST_PS2);
  outb(PS2_CMD, PS2_CONFIGURATION_BYTE);
  config_byte = inb(PS2_DATA);
  if (!((config_byte >> 4) & 1)) {
    ps2_channel1_available = true;
  }

  outb(PS2_CMD, ENABLE_SECOND_PS2);
  outb(PS2_CMD, PS2_CONFIGURATION_BYTE);
  config_byte = inb(PS2_DATA);
  if (!((config_byte >> 5) & 1)) {
    ps2_channel2_available = true;
  }

  printf("PS2 Channel 1: %d, PS2 Channel 2: %d\n", 100, ps2_channel2_available);
  return true;
}

uint8_t ps2_send_byte(uint8_t port_id, uint8_t byte)
{
  if (port_id == 1 && !ps2_channel1_available) { return; }
  if (port_id == 2 && !ps2_channel2_available) { return; }

  return 0;
}

// Polls till the current op is complete by the PS2 controller
void ps2_poll()
{
  // for (int i = 50000; i >= 0; i--) {
  //   uint8_t status = inb(PS2_DATA);
  //   if ((status & 1) == 0) {
  //     break;
  //   }
  // }
}
