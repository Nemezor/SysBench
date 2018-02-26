[BITS 64]

global memory_clear

; void memory_clear(uint64_t start, uint64_t size);
memory_clear:
    xor rax, rax
.repeat:
    mov [rdi], al
    dec rsi
    jnz .repeat
    ret
