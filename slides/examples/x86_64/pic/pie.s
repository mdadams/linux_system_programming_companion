	.text
# extern "C" void _start()
	.globl _start
_start:
	call main
	mov %rax, %rdi
	call exit # does not return
# extern "C" int main()
	.globl main
main:
	mov answer(%rip), %rax # NOTE: THIS LINE CHANGED!
	ret
# extern "C" void exit(int status)
	.globl exit
exit:
	.set SYS_exit, 60 # system call number for exit
	mov $SYS_exit, %rax
	syscall # does not return
	.data
# int answer = 42;
	.globl answer
answer:
	.align 4
	.long 0x2a # 42
