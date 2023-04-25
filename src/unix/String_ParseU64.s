.text
.globl String_ParseU64
.type String_ParseU64,@function
String_ParseU64:
.cfi_startproc
movq (%rsi), %rcx
movq $0x1999999999999999, %rdx
xorq %rax, %rax

.NEXT:
movb (%rcx), %al
addb $-48, %al
cmpb $10, %al
jae .NOT_DIGIT
cmpq %rdx, %rdi
jae .MAYBE_OVERFLOW
leaq (%rdi, %rdi, 4), %rdi
leaq (%rax, %rdi, 2), %rdi
addq $1, %rcx
jmp .NEXT

.NOT_DIGIT:
movq %rcx, (%rsi)
movq %rdi, %rax
retq

.MAYBE_OVERFLOW:
jne .OVERFLOW
cmpb $6, %al
jae .OVERFLOW
movq $0xFFFFFFFFFFFFFFFA, %rdi
addq %rax, %rdi
addq $1, %rcx
jmp .NEXT

.OVERFLOW:
movq %rcx, (%rsi)
movq $-1, %rax
retq
.cfi_endproc

.section	".note.GNU-stack","",@progbits
