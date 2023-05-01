.text
.globl String_ParseU64
.type String_ParseU64,@function
String_ParseU64:
.cfi_startproc
movq (%rsi), %rcx
movq %rdi, %rax
movq $0x1999999999999999, %rdi

.NEXT:
movzbl (%rcx), %edx
addb $-48, %dl
cmpb $10, %dl
jae .NOT_DIGIT
cmpq %rdi, %rdx
jae .MAYBE_OVERFLOW
leaq (%rax, %rax, 4), %rax
leaq (%rdx, %rax, 2), %rax
addq $1, %rcx
jmp .NEXT

.NOT_DIGIT:
movq %rcx, (%rsi)
retq

.MAYBE_OVERFLOW:
jne .OVERFLOW
cmpb $6, %dl
jae .OVERFLOW
movq $0xFFFFFFFFFFFFFFFA, %rax
addq %rdx, %rax
addq $1, %rcx
jmp .NEXT

.OVERFLOW:
movq %rcx, (%rsi)
movq $-1, %rax
retq
.cfi_endproc

.section	".note.GNU-stack","",@progbits
