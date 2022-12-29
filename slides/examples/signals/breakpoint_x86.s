	.text
	.globl breakpoint
# void breakpoint(int type)
breakpoint:
	test %edi, %edi
	jnz .L0
	int3 # 1-byte breakpoint opcode (0xcc)
	jmp .L1
.L0: # 2-byte breakpoint opcode
	.byte 0xcd, 0x03
.L1:
	ret
