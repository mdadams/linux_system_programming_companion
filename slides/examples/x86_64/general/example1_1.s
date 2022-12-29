	.text
	.globl	_Z3bariii
# int bar(int a, int b, int c)
_Z3bariii:
	subq	$24, %rsp
	movl	%edi, 12(%rsp) # a
	leaq	12(%rsp), %rdi
	movl	%esi, 8(%rsp) # b
	leaq	8(%rsp), %rsi
	movl	%edx, 4(%rsp) # c
	leaq	4(%rsp), %rdx
	call	_Z3fooRiS_S_
	movl	8(%rsp), %eax # ret = b
	addl	12(%rsp), %eax # ret += a
	addl	4(%rsp), %eax # ret += c
	addq	$24, %rsp # free space
	ret
