[BITS 64]

global swrender_framebuffercpy

; void swrender_framebuffercpy(void* back, void* front, void* destination, uint64_t size);
swrender_framebuffercpy:
.repeat:
    mov rax, [rdi]
    mov r8, [rsi]
    cmp rax, r8
    je .jump_over
    mov [rdx], rax
    mov [rsi], rax
.jump_over:
    add rdi, 8
    add rsi, 8
    add rdx, 8
    dec rcx
    jnz .repeat
    ret
