	.text
	.globl	_Z6do_addv
_Z6do_addv:
	# int add(1, 2, 3, 4, 5, -5, -4, -3, -2, -1)
	# note: x86-64 cannot push 32-bit register
	pushq $-1 # 10th argument
	pushq $-2 # 9th argument
	pushq $-3 # 8th argument
	pushq $-4 # 7th argument
	mov $-5, %r9d # 6th argument
	mov $5, %r8d # 5th argument
	mov $4, %ecx # 4th argument
	mov $3, %edx # 3rd argument
	mov $2, %esi # 2nd argument
	mov $1, %edi # 1st argument
	call _Z3addiiiiiiiiii
	add $32, %rsp # remove arguments from stack
	ret
