# note: sizeof(unsigned int) is 4
.text
.globl _Z14hamming_weightj
# unsigned int hamming_weight(unsigned int)
_Z14hamming_weightj:
	# note: xor %eax, %eax has shorter opcode than mov $0, %eax
	xorl %eax, %eax
.L_loop_start:
	# note: test %edi, %edi has shorter opcode than cmp $0, %edi
	test %edi, %edi # edi & edi = edi = ?
	jz .L_loop_end # branch if edi == 0
	mov %edi, %edx
	and $1, %edx
	add %edx, %eax
	shr %edi
	jmp .L_loop_start
.L_loop_end:
	ret
