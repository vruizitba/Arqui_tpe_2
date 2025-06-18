EXTERN registers_state

GLOBAL _get_registers

SECTION .text

_get_registers:
    push rbp
    mov rbp, rsp

    mov rcx, 17
    mov rsi, registers_state
    rep movsq

    mov rsp, rbp
    pop rbp
    ret