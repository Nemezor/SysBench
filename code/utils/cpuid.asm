[BITS 64]

global cpuid_query

; void cpuid_query(struct cpuid* cpuid_struct)
cpuid_query:
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, [rdi]
    mov rbx, [rdi + 8]
    mov rcx, [rdi + 16]
    mov rdx, [rdi + 24]

    cpuid

    mov [rdi], rax
    mov [rdi + 8], rbx
    mov [rdi + 16], rcx
    mov [rdi + 24], rdx

    pop rdx
    pop rcx
    pop rbx
    pop rax
    ret
