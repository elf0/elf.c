#License: Public Domain
#Author: elf
#EMail: elf198012@gmail.com

.text
.globl U64_Parse, UI_Add, UI_Sub

U64_Parse:
movq %rdx, %r8
movq %rcx, %r9
xorq %rax, %rax
xorq %rcx, %rcx
xorb %dl, %dl
movq $10, %rbx
0:
movb (%rdi), %cl
subb $0x30, %cl
js 1f
cmpb $9, %cl
jg 1f
mulq %rbx
setcb %dl
jc 2f
addq %rcx, %rax
setcb %dl
jc 2f
incq %rdi
decq %rsi
jnz 0b
1:
movq %rax, (%r8)
2:
movb %dl, (%r9)
movq %rdi, %rax
ret

UI_Add:
movq (%rsi), %rax
addq %rax, (%rdi)
0:
leaq 8(%rdi), %rdi
leaq 8(%rsi), %rsi
movq (%rsi), %rax
adcq %rax, (%rdi)
decq %rdx
jnz 0b
setcb %al
ret

UI_Sub:
movq (%rsi), %rax
subq %rax, (%rdi)
0:
leaq 8(%rdi), %rdi
leaq 8(%rsi), %rsi
movq (%rsi), %rax
sbbq %rax, (%rdi)
decq %rdx
jnz 0b
setcb %al
ret

