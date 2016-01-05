.global gdt_flush
.global stop_interrupts

gdt_flush:
	cli
	mov 4(%esp), %eax
	lgdt (%eax)

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	xchgw %bx, %bx

  jmp $0x08, $.flush		# far jump to populate cs

.flush:
	ret

stop_interrupts:
	cli

.section .data:
	hello: .asciz "Ping\n"
