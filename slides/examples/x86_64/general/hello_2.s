	.text
	.globl _start
_start:
	# n = write(1, hello, hello_len)
	mov $1, %rdi
	mov $hello, %rsi
	mov $hello_len, %rdx
	call write
	# exit(n == hello_len ? 0 : 1)
	mov $1, %rdi
	cmp $hello_len, %rax # rax - $hello_len = ?
	jne .L0 # branch if rax - $hello_len != 0 (i.e., rax != $hello_len)
	mov $0, %rdi
.L0:
	call exit # does not return
write:
	.set SYS_write, 1 # system call number for write
	mov $SYS_write, %rax
	syscall
	ret
exit:
	.set SYS_exit, 60 # system call number for exit
	mov $SYS_exit, %rax
	syscall # does not return
	.data
hello:
	.ascii "Hello, World!\n"
	.set hello_len, . - hello
