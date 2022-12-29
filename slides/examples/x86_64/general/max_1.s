# note: sizeof(int) is 4
	.text
	.globl _Z3maxii
# int max(int, int)
_Z3maxii:
	cmp %edi, %esi # esi - edi = ?
	jle .L0 # branch if esi - edi <= 0 (i.e., esi <= edi)
	mov %esi, %eax # set return value to esi
	jmp .L1
.L0:
	mov %edi, %eax # set return value to edi
.L1:
	ret
