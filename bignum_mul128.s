global bignum_mul128

section .text

bignum_mul128:
    ; rdi = a, rsi = b, rdx = result
    push rbx
    push r12

    mov r12, rdx        ; simpan result ptr

    mov rax, [rdi]      ; a0
    mov rbx, [rdi+8]    ; a1
    mov rcx, [rsi]      ; b0
    mov r8,  [rsi+8]    ; b1

    ; a0 * b0
    mov rax, [rdi]
    mul rcx             ; rdx:rax = a0*b0
    mov [r12], rax      ; result[0]
    mov r9, rdx         ; tmp

    ; a0 * b1
    mov rax, [rdi]
    mul r8              ; rdx:rax = a0*b1
    add rax, r9         ; + high(z0)
    mov r10, rdx
    adc r10, 0

    ; a1 * b0
    mov rax, rbx
    mul rcx
    add rax, r10
    mov [r12+8], rax    ; result[1]
    mov r11, rdx
    adc r11, 0

    ; a1 * b1
    mov rax, rbx
    mul r8
    add rax, r11
    adc rdx, 0
    mov [r12+16], rax   ; result[2]
    mov [r12+24], rdx   ; result[3]

    pop r12
    pop rbx
    ret

