#include "../include/kernel/gdt.h"
#include <stdint.h>
#include <stdlib.h>
#include <log.h>
#include <stdio.h>

struct GDTDescr {
  uint16_t limit_low; // The lower 16 bits of the limit
  uint16_t base_low;
  uint16_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
}__attribute__((packed));

// granularity byte format:
// 7 6 5 4 3 2 1 0
// G D 0 A Segment length 19:16
struct GDTPtr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));


typedef struct GDTPtr gdt_ptr_t;
typedef struct GDTDescr gdt_entry_t;

extern void gdt_flush(uint32_t);
static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);
static void print_gdt();

gdt_entry_t gdt_entries[3];
gdt_ptr_t   gdt_ptr;

void load_gdt() {
  gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
  gdt_ptr.base  = (uint32_t)&gdt_entries;

  gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
  // gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
  // gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

  // print_gdt();

  gdt_flush((uint32_t)&gdt_ptr);
}

// static void print_gdt() {
//   for (int i = 0; i < 5; i++) {
//     print_mem(&gdt_entries[i], sizeof(gdt_entry_t));
//   }
// }

// Set the value of one GDT entry.
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}
