# note: sizeof(unsigned long long) is 8
	.text
	.globl _Z9factorialy
# unsigned long long factorial(unsigned long long)
_Z9factorialy:
	# note: clears upper 32 bits of eax
	mov $1, %eax
.L_loop_start:
	cmp $1, %rdi # rdi - 1 = ?
	jbe .L_loop_end # branch if rdi - 1 <= 0 (i.e., rdi <= 1)
	# note: signed multiply gives correct lower 64-bits of result
	imul %rdi, %rax
	sub $1, %rdi
	jmp .L_loop_start
.L_loop_end:
	ret
