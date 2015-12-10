#include "../include/kernel/idt.h"
#include <stdio.h>
#include <stdint.h>

struct IDTDescr {
  uint16_t offset_1;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_2;
};

void load_idt() {
  
}
