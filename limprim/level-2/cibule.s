.intel_syntax noprefix
.global _start

.section .data

msg:
.ascii "Input your cibule:\n"
.set msg_len, . - msg

flag_path:
.ascii "/flag\0"

sockaddr:
    .word 0x2
    .word 0x6f1a
    .long 0x0
    .quad 0x0

filter:
    .short 0x20
    .byte 0x0
    .byte 0x0
    .long 0x0

    .short 0x15
    .byte 0x1
    .byte 0x0
    .long 0x0

    .short 0x6
    .byte 0x0
    .byte 0x0
    .long 0x80000000

    .short 0x6
    .byte 0x0
    .byte 0x0
    .long 0x7fff0000

.section .text

_start:
    mov rax, 0x29
    mov rdi, 0x2
    mov rsi, 0x1
    xor rdx, rdx
    syscall

    mov r12, rax
    
    push 0x1
    mov rdx, 0x2
    mov rsi, 0x1
    mov rdi, r12
    mov r10, rsp
    mov r8, 0x4
    mov rax, 0x36
    syscall
    pop rax

    jmp bind

wait_bind:
    xor rcx, rcx
    mov rax, 0x10000000
wait_bind_loop:
    inc rcx
    cmp rcx, rax
    jb wait_bind_loop
bind:
    mov rax, 0x31
    mov rdi, r12
    lea rsi, [rip+sockaddr]
    mov rdx, 0x10
    syscall
    cmp rax, 0
    jnz wait_bind

    mov rax, 0x32
    mov rdi, r12
    mov rsi, 0x1
    syscall

    mov rax, 0x2b
    mov rdi, r12
    xor rsi, rsi
    xor rdx, rdx
    syscall

    mov r13, rax

    lea rdi, [rip+flag_path]
    xor rsi, rsi
    mov rax, 0x2
    syscall

    mov r14, rax

    mov rdi, r13
    lea rsi, [rip+msg]
    mov rdx, msg_len
    mov rax, 0x1
    syscall

    lea rdi, [rip+_start]
    mov rsi, 0x1000
    mov rdx, 0x7
    mov rax, 0xa
    syscall

    mov rdi, 0x26
    mov rsi, 0x1
    xor rdx, rdx
    xor r10, r10
    xor r8, r8
    mov rax, 0x9d
    syscall

    sub rsp, 0x10

    mov word ptr [rsp], 0x4
    lea rax, [rip+filter]
    mov [rsp+0x8], rax

    mov rdi, 0x16
    mov rsi, 0x2
    mov rdx, rsp
    mov rax, 0x9d
    syscall

    mov rdi, r13
    lea rsi, [rip+user_input]
    mov rdx, 0x3
    xor rax, rax
    syscall

user_input:
    .string "Goodbye!"
