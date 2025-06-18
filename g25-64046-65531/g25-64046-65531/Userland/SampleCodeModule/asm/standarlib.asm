GLOBAL _syscall:


section .text

_syscall:
    push rbp
    mov rbp, rsp

    push rbx

    mov r10, rcx ;copia del 3er parametro

    mov rax, rdi
    mov rbx, rsi
    mov rcx, rdx
    mov r8, r10

    int 80h

    pop rbx

    mov rsp, rbp
    pop rbp

    ret