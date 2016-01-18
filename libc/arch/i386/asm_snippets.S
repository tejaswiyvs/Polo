.global gdt_flush
.global idt_flush
.global cpu_reset
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

	# Load up the segment registers with 0x10
	# 0x10 = dec 16
	# GDT looks like this:
	# address 0: Null
	# address 8: Code Segment
	# address 16: Data segment
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	# We cannot directly load cs. So far jump to populate it instead.
  jmp $0x08, $.flush

.flush:
		# No need to start interrupts again because the OS will do so when it sets up
		# the IDT
		ret

idt_flush:
	mov 4(%esp), %eax
	lidt (%eax)
	ret

cpu_reset:
	# Wait for a empty Input Buffer
	.wait1:
		in   $0x64, %al
		test $0x2, %al
		jmp  .wait1

	# Send 0xFE to the keyboard controller.
	mov  $0xFE, %al
	out  %al, $0x64
	ret
