	.text
	.globl func
func:
	# size of storage (in bytes) for local variables, which
	# must be odd multiple of 8 if any function calls made
	.set local_size, 64
	# allocate storage for local variables
	sub $local_size, %rsp
	# locals at 0(%rsp) to local_size-1(%rsp)
	# return address at local_size(%rsp)
	# arguments passed on stack (if any) start at local_size+8(%rsp)
	# Note that the addresses of the arguments depend on rsp and
	# local_size, and the addresses of local variables depend on rsp.
	# These dependencies are often undesirable.
	# ... (do something useful)
	add $local_size, %rsp
	ret
