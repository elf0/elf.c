section .text
global I128_Add, I128_AddMR, I128_AddRR, I128_Sub, I128_SubMR, I128_SubRR, I128_Mul, I128_MulMR, I128_MulRR
I128_Add:
 mov rax, qword[rdi]
 mov rcx, qword[rdi + 8]
 add rax, qword[rsi]
 adc rcx, qword[rsi + 8]
 mov qword[rdx], rax
 mov qword[rdx + 8], rcx
 ret

I128_AddMR:
 add rdi, rdx
 adc rsi, rcx
 mov qword[r8], rdi
 mov qword[r8 + 8], rsi
 ret

I128_AddRR:
 add rdi, rdx
 adc rsi, rcx
 mov rdx, rsi
 mov rax, rdi
 ret

I128_Sub:
 mov rax, qword[rdi]
 mov rcx, qword[rdi + 8]
 sub rax, qword[rsi]
 sbb rcx, qword[rsi + 8]
 mov qword[rdx], rax
 mov qword[rdx + 8], rcx
 ret

I128_SubMR:
 sub rdi, rdx
 sbb rsi, rcx
 mov qword[r8], rdi
 mov qword[r8 + 8], rsi
 ret

I128_SubRR:
 sub rdi, rdx
 sbb rsi, rcx
 mov rdx, rsi
 mov rax, rdi
 ret

I128_Mul:
 mov r8, rdx
 mov rax, qword[rdi]
 mov rcx, qword[rsi]
 mov rsi, qword[rsi + 8]
 imul rsi, rax
 mul rcx
 imul rcx, qword[rdi + 8]
 mov qword[r8], rax
 add rdx, rsi
 add rdx, rcx
 mov qword[r8 + 8], rdx
 ret

I128_MulMR:
 mov rax, rdi
 imul rsi, rdx
 imul rdi, rcx
 mul rdx
 mov qword[r8], rax
 add rdx, rsi
 add rdx, rdi
 mov qword[r8 + 8], rdx
 ret

I128_MulRR:
 mov rax, rdi
 imul rsi, rdx
 imul rdi, rcx
 mul rdx
 add rdx, rsi
 add rdx, rdi
 ret

