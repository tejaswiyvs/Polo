.global gdt_flush
.global idt_flush
# The GDT is pointed to by a special register in the x86 chip, the GDTR register
# The GDTR is 48 bits long.
#   - Lower 16 bits = Size of the GDT (number of entries in the GDT)
#   - Upper 32 bits = Location of the GDT in memory
# gdt_flush expects the GDTR fully populated via a packed struct
# invoke using gdt_flush(&gdtr_address);
gdt_flush:
	cli
	mov 4(%esp), %eax
	lgdt (%eax)
	ret
	# The code below is useful if our kernel loads up in the higher half.
	# since we just have a bare bones kernel that GRUB loaded, this isn't necessary.

	# mov $0x10, %ax
	# mov %ax, %ds
	# mov %ax, %es
	# mov %ax, %fs
	# mov %ax, %gs
	# mov %ax, %ss

	# xchgw %bx, %bx

  # ljmp $0x08, $.flush		# far jump to populate cs

idt_flush:
	xchgw %bx, %bx
	mov 4(%esp), %eax
	lidt (%eax)
	ret

.flush:
	# No need to start interrupts again because the OS will do so when it sets up
	# the IDT
	ret
