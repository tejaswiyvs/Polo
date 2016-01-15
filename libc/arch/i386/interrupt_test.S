int_handler:
    xchgw %bx, %bx
    movw $0x123, 0x0 // this places magic value "0x123abc" at the beginning of memory
    hlt

idt:
   .skip 50*2

idtr:
   .short (50*8)-1
   .long idt

.global do_test
do_test:
  xchgw %bx, %bx
  lidt idtr
	mov $int_handler, %eax
	mov %ax, idt+49*8
	movw $0x08, idt+49*8+2
	movw $0x8E00, idt+49*8+4
	shr $16, %eax
	mov %ax, idt+49*8+6
  int $49
