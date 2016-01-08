.macro  ISR_NOERRCODE 1 # A macro for ISRs that don't push an error code
  .global isr\1
  isr\1:
    cli
    push byte 0
    push byte \1
    jmp isr_common_stub
.endm

.macro  ISR_ERRORCODE 1 # A macro for ISRs that do push an error code
  .global isr\1
  isr\1:
    cli
    push byte \1
    jmp isr_common_stub
.endm

# This is in isr.c
.extern isr_handler

# This is our common isr stub. It saves the processor state, sets up for kernel
# mode segments, calls the C-level fault handler, and finally restores the stack
# frame
isr_common_stub:
  pusha           # push all general purpose registers onto the stack
  mov %ds, %ax
  push %eax

  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  call isr_handler

  pop %eax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  popa           # Previously we've pushed all the GPRs onto the stack using pusha. Restore all of those using popa
  add $8, %esp
  sti
  iret
