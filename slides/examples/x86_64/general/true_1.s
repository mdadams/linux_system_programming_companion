	.set SYS_exit, 60 # system call number for exit
	.text
	.globl _start
# _start is linker's default entry point for program
_start:
	# exit(0)
	mov $0, %edi # note: xor %edi, %edi would have shorter opcode
	mov $SYS_exit, %rax
	syscall
	# not reached
