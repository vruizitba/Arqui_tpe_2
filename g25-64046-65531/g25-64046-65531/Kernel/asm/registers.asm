EXTERN registers_state
EXTERN userland_registers_state
EXTERN userland_registers_state_aux

GLOBAL _get_registers
GLOBAL _get_registers_userland
GLOBAL _update_registers_userland

SECTION .text

%macro copyRegisters 1
    mov rcx, 19
    mov rsi, %1
    rep movsq
%endmacro

_get_registers:
    push rbp
    mov rbp, rsp

    copyRegisters registers_state

    mov rsp, rbp
    pop rbp
    ret

_get_registers_userland:
    push rbp
    mov rbp, rsp

    copyRegisters userland_registers_state

    mov rsp, rbp
    pop rbp
    ret:

_update_registers_userland:
    push rbp
    mov rbp, rsp

    mov rdi, userland_registers_state
    copyRegisters userland_registers_state_aux

    mov rsp, rbp
    pop rbp
    ret