	.set SYS_write, 1
	.set SYS_exit, 60
	.set hello_len, hello_end - hello - 1
	.text
	.globl _start
_start:
	mov $1, %rdi
	mov $hello, %rsi # for PIE: lea hello(%rip), %rsi
	mov $hello_len, %rdx
	mov $SYS_write, %rax
	syscall # n = write(1, hello, hello_len)
	# n == hello_len ? 0 : 1
	mov $1, %rdi # status = 1
	cmp $hello_len, %rax
	jne .L_skip
	mov $0, %rdi # status = 0
.L_skip:
	mov $SYS_exit, %rax
	syscall # exit(status)
	.data
hello:
	.string "Hello, World!\n"
hello_end:
