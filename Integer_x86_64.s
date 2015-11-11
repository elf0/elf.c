.text
.globl UI_Add
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
retq
