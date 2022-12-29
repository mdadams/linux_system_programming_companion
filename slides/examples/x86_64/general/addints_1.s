	.text
	.globl	_Z3addiiiiiiiiii
# int add(int a0, int a1, int a2, int a3,
#   int a4, int a5, int a6, int a7, int a8,
#   int a9)
_Z3addiiiiiiiiii:
	mov %edi, %eax # result = a0
	add	%esi, %eax # result += a1
	add	%edx, %eax # result += a2
	add	%ecx, %eax # result += a3
	add	%r8d, %eax # result += a4
	add	%r9d, %eax # result += a5
	add	8(%rsp), %eax # result += a6
	add	16(%rsp), %eax # result += a7
	add	24(%rsp), %eax # result += a8
	add	32(%rsp), %eax # result += a9
	ret
