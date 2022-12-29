	.text
	.globl func
func:
	# size of storage (in bytes) for local variables, which
	# must be multiple of 16 if any function calls made
	.set local_size, 64
	# establish rbp as frame pointer
	push %rbp
	mov %rsp, %rbp
	# allocate storage for local variables
	sub $local_size, %rsp
	# locals at -local_size(%rbp) to -1(%rbp)
	# saved rbp at 0(%rbp)
	# return address at 8(%rbp)
	# arguments passed on stack (if any) start at 16(%rbp)
	# Note that the addresses of function arguments
	# and local variables depend neither on rsp
	# nor local_size, which is often desirable.
	# ... (do something useful)
	leave
	# leave is equivalent to:
	# mov %rbp, %rsp
	# pop %rbp
	ret
