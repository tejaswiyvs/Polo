.global gdt_flush
.global stop_interrupts

gdt_flush:
	push $10
	call logv

	mov 4(%esp), %eax
	lgdt (%eax)

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

  jmp $0x08, $.flush		# far jump to populate cs

.flush:
	ret

stop_interrupts:
	cli
