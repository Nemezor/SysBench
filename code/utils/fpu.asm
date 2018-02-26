[BITS 64]

global fpu_init_x87
global fpu_init_sse
global fpu_save
global fpu_load

; void fpu_init_x87();
fpu_init_x87:
    fninit
    ret

; void fpu_init_sse();
fpu_init_sse:
    mov rax, cr0
    and ax, 0xFFFB
    or ax, 0x2
    mov cr0, rax
    mov rax, cr4
    or ax, 3 << 9
    mov cr4, rax
    ret

; void fpu_save(void* data);
fpu_save:
    fxsave [rdi]
    ret

; void fpu_load(void* data);
fpu_load:
    fxrstor [rdi]
    ret
