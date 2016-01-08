.macro ISR_NOERRCODE int_no # A macro for ISRs that don't push an error code
  .global isr\int_no
  isr\int_no:
    cli
    pushw 0
    pushw \int_no
    jmp isr_common_stub
.endm

.macro ISR_ERRORCODE int_no # A macro for ISRs that do push an error code
  .global isr\int_no
  isr\int_no:
    cli
    pushw \int_no
    jmp isr_common_stub
.endm


ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRORCODE 8  # ISR 8 pushes error code onto stack
ISR_NOERRCODE 9
ISR_ERRORCODE 10 # ISR 10 - 14 push error codes onto stack
ISR_ERRORCODE 11
ISR_ERRORCODE 12
ISR_ERRORCODE 13
ISR_ERRORCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

# This is in isr.c
.extern isr_handler

# This is our common isr stub. It saves the processor state, sets up for kernel
# mode segments, calls the C-level fault handler, and finally restores the stack
# frame
isr_common_stub:
  pusha           # push all general purpose registers onto the stack
  mov %ds, %ax
  push %eax

  # mov $0x10, %ax
  # mov %ax, %ds
  # mov %ax, %es
  # mov %ax, %fs
  # mov %ax, %gs

  call isr_handler

  pop %eax
  # mov %ax, %ds
  # mov %ax, %es
  # mov %ax, %fs
  # mov %ax, %gs

  popa           # Previously we've pushed all the GPRs onto the stack using pusha. Restore all of those using popa
  add $8, %esp
  sti
  iret
