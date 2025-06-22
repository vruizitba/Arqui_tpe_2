
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _syscallHandler


GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
extern syscall_dispatcher

GLOBAL registers_state
GLOBAL userland_registers_state
GLOBAL userland_registers_state_aux

ALIGN 8

SECTION .bss
registers_state: resq 19
userland_registers_state: resq 19
userland_registers_state_aux: resq 19
aux resq 1

SECTION .text

%macro saveRegisters 1
    mov [%1 + 0*8], rax
    mov [%1 + 1*8], rbx
    mov [%1 + 2*8], rcx
    mov [%1 + 3*8], rdx
    mov [%1 + 4*8], rbp
    mov rax, [rsp+8*3]      ; rsp anterior a la llamada
    mov [%1 + 5*8], rax
    mov [%1 + 6*8], rsi
    mov [%1 + 7*8], rdi
    mov [%1 + 8*8], r8
    mov [%1 + 9*8], r9
    mov [%1 + 10*8], r10
    mov [%1 + 11*8], r11
    mov [%1 + 12*8], r12
    mov [%1 + 13*8], r13
    mov [%1 + 14*8], r14
    mov [%1 + 15*8], r15
    mov rax, [rsp]          ; rip userland
    mov [%1 + 16*8], rax
    mov rax, [rsp+8*2]      ; rflags
    mov [%1 + 17*8], rax
    mov rax, [rsp+8]        ; cs
    mov [%1 + 18*8], rax
    mov rax, [%1 + 0*8]          ; restauro rax
%endmacro

_syscallHandler:
    pushState

    mov rdi, rax
    mov rsi, rbx
    mov rdx, rcx
    mov rcx, r8

    call syscall_dispatcher

    popState
    iretq

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	saveRegisters registers_state
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState

    add qword [rsp], 2

	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
    saveRegisters userland_registers_state_aux
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
    exceptionHandler 6

haltcpu:
	cli
	hlt
	ret
