section .text
global I128_Add, I128_AddMR, I128_AddRR, I128_Sub, I128_SubMR, I128_SubRR, I128_Mul, I128_MulMR, I128_MulRR
I128_Add:
 mov rax, QWORD[rdi]
 mov rcx, QWORD[rdi + 8]
 add rax, QWORD[rsi]
 adc rcx, QWORD[rsi + 8]
 mov QWORD[rdx], rax
 mov QWORD[rdx + 8], rcx
 ret

I128_AddMR:
 add rdi, rdx
 adc rsi, rcx
 mov QWORD[r8], rdi
 mov QWORD[r8 + 8], rsi
 ret

I128_AddRR:
 add rdi, rdx
 adc rsi, rcx
 mov rdx, rsi
 mov rax, rdi
 ret

I128_Sub:
 mov rax, QWORD[rdi]
 mov rcx, QWORD[rdi + 8]
 sub rax, QWORD[rsi]
 sbb rcx, QWORD[rsi + 8]
 mov QWORD[rdx], rax
 mov QWORD[rdx + 8], rcx
 ret

I128_SubMR:
 sub rdi, rdx
 sbb rsi, rcx
 mov QWORD[r8], rdi
 mov QWORD[r8 + 8], rsi
 ret

I128_SubRR:
 sub rdi, rdx
 sbb rsi, rcx
 mov rdx, rsi
 mov rax, rdi
 ret

I128_Mul:
 mov rcx, QWORD[rsi]
 mov rsi, QWORD[rsi + 8]
 mov rax, QWORD[rdi]
 imul rsi, rax
 mov r8, rdx
 mul rcx
 imul rcx, QWORD[rdi + 8]
 add rdx, rcx
 add rdx, rsi
 mov QWORD[r8], rax
 mov QWORD[r8 + 8], rdx
 ret

I128_MulMR:
 mov rax, rdi
 imul rdi, rcx
 imul rsi, rdx
 mul rdx
 add rdx, rsi
 add rdx, rdi
 mov QWORD[r8], rax
 mov QWORD[r8 + 8], rdx
 ret

I128_MulRR:
 mov rax, rdi
 imul rdi, rcx
 imul rsi, rdx
 mul rdx
 add rdx, rsi
 add rdx, rdi
 ret

