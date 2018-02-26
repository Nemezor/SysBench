[BITS 64]

global gdt_read_ptr
global gdt_read_size

; uint64_t gdt_read_ptr();
gdt_read_ptr:
    xor rax, rax
    push rax
    push rax
    sgdt [rsp]
    add rsp, 2
    mov rcx, [rsp]
    sub rsp, 2
    pop rax
    pop rax
    mov rax, rcx
    ret

; uint16_t gdt_read_size();
gdt_read_size:
    xor rax, rax
    push rax
    push rax
    sgdt [rsp]
    mov rcx, [rsp]
    pop rax
    pop rax
    mov rax, rcx
    ret
