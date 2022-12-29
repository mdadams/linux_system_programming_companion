	.text
	.globl safe_read
	.globl safe_read_ins
safe_read:
	mov $0, %eax
safe_read_ins:
	mov (%rdi), %al
.L0:
	# upon failure, %eax set to -1
	ret
	.data
	.globl safe_read_ins_len
safe_read_ins_len:
	.long .L0 - safe_read_ins
