[BITS 64]

global idt_load
global idt_get_idtr

; void idt_load(uint64_t base, int32_t size);
idt_load:
    xor rax, rax
    push rax
    push rax
    mov [rsp + 2], rdi
    mov [rsp], si
    lidt [rsp]
    pop rax
    pop rax
    ret

; uint64_t idt_get_idtr();
idt_get_idtr:
    xor rax, rax
    push rax
    push rax
    sidt [rsp]
    add rsp, 2
    mov rcx, [rsp]
    sub rsp, 2
    pop rax
    pop rax
    mov rax, rcx
    ret
