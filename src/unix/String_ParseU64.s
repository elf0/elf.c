.text
.globl String_ParseU64
.type String_ParseU64,@function
String_ParseU64:
.cfi_startproc
movq (%rsi), %rcx
movzbq (%rcx), %rax
movq $0x1999999999999999, %r8
addb $-48, %al
movq %rax, %rdx

.NEXT:
addq $1, %rcx
movb (%rcx), %al
addb $-48, %al
cmpb $10, %al
jae .NOT_DIGIT
cmpq %r8, %rdx
jae .MAYBE_OVERFLOW
leaq (%rdx, %rdx, 4), %rdx
leaq (%rax, %rdx, 2), %rdx
jmp .NEXT

.NOT_DIGIT:
# cmpb $0xF7, %al
# je .NEXT
movq %rdx, (%rdi)
movq %rcx, (%rsi)
xorl	%eax, %eax
retq

.MAYBE_OVERFLOW:
jne .OVERFLOW
cmpb $6, %al
jae .OVERFLOW
movq $0xFFFFFFFFFFFFFFFA, %rdx
addq %rax, %rdx
jmp .NEXT

.OVERFLOW:
movq %rdx, (%rdi)
movq %rcx, (%rsi)
movl $1, %eax
retq
.cfi_endproc

.section	".note.GNU-stack","",@progbits
