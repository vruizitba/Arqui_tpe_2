GLOBAL outb
GLOBAL inb
GLOBAL outw

section .text

inb:
    push rbp
    mov rbp, rsp

    mov dx, di
    in al, dx
    movzx rax, al

    mov rsp, rbp
    pop rbp
    ret

outb:
    push rbp
    mov rbp, rsp

    mov dx, di
    mov al, sil
    out dx, al

    mov rsp, rbp
    pop rbp
    ret

outw:
    push rbp
    mov rbp, rsp

    mov dx, di
    mov ax, si
    out dx, ax

    mov rsp, rbp
    pop rbp
    ret