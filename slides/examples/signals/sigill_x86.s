	.text
	.globl illegal_instruction
# void illegal_instruction(int type)
illegal_instruction:
	test %eax, %eax
	jnz .L1
	ud2 # illegal instruction (2-byte opcode: 0x0f, 0x0b)
	jmp .L_done
.L1:
	cmp $1, %eax
	jnz .L2
	ud0 %rax, %rax # illegal instruction (4-byte opcode: 0x48, 0x0f, 0xff, 0xc0)
	jmp .L_done
.L2:
	cmp $2, %eax
	jnz .L_done
	ud1 %rax, %rax # illegal instruction (4-byte opcode: 0x48, 0x0f, 0xb9, 0xc0)
.L_done:
	ret
