[BITS 64]

global msr_write
global msr_read

; void msr_write(uint64_t data, uint64_t address)
msr_write:
    push rcx
    push rdx

    mov rcx, rdi
    mov rax, rsi
    mov rdx, rsi
    shr rdx, 32
    wrmsr

    pop rdx
    pop rcx
    ret

; uint64_t msr_read(uint64_t address)
msr_read:
    push rcx
    push rdx

    mov rcx, rdi
    rdmsr
    shl rdx, 32
    mov rcx, 0xFFFFFFFF
    and rax, rcx
    or rax, rdx

    pop rdx
    pop rcx
    ret
