[BITS 32]

section .multiboot
    ; multiboot header
    align 4
    dd 0x1BADB002
    ; graphical
    dd 0x06
    dd -(0x1BADB002 + 0x06)
	times 5 dd 0
    dd 0
    dd 1920
    dd 1080
    dd 32
    ; textmode
    ;dd 0x02
    ;dd -(0x1BADB002 + 0x02)
    ;times 9 dd 0

section .text

global __start__

extern __setup_kernel_entry

mem_eax:
    dd 0
mem_ebx:
    dd 0

; Entry point of the system
__start__:
    cli
    mov esp, reserved_stack_space
    mov ebp, reserved_stack_space
    mov [mem_eax], eax
    mov [mem_ebx], ebx

    lidt [entry_idt64]
.set_PML4T:
    mov eax, PDPT
    or eax, 3
    mov ebx, PML4T
    mov [ebx], eax
.set_PDPT:
    mov eax, PDT
    or eax, 3
    mov ebx, PDPT
.PDPT_loop:
    mov [ebx], eax
    add ebx, 8
    add eax, 4096
    cmp ebx, PDPT+32
    jl .PDPT_loop
.set_PDT:
    mov eax, PT
    or eax, 3
    mov ebx, PDT
.PDT_loop:
    mov [ebx], eax
    add ebx, 8
    add eax, 4096
    cmp ebx, PDT+16384
    jl .PDT_loop
.set_PT:
    mov eax, 3
    mov ebx, PT
.PT_loop:
    mov ecx, 0
    cmp eax, 0xFEE00000
    jl .skip_cache
    cmp eax, 0x100000
    jge .skip_cache
    mov ecx, 0x10
.skip_cache:
    mov [ebx], eax
    or [ebx], ecx
    add ebx, 8
    add eax, 4096
    cmp ebx, PT+8388608
    jl .PT_loop

    mov eax, PML4T
    mov cr3, eax

    mov eax, cr4
    or eax, 0xA0
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr

    or eax, 0x00000100
    wrmsr

    mov ebx, cr0
    or ebx, 0x80000000
    mov cr0, ebx

    lgdt [entry_gdt64.gdt_pointer]

    jmp entry_gdt64.gdt_code:entry_long_mode

[BITS 64]
entry_long_mode:
    cli
    mov ax, entry_gdt64.gdt_data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, reserved_stack_space
    mov rbp, reserved_stack_space

    mov eax, [mem_eax]
    mov ebx, [mem_ebx]
    mov rdi, rbx
    mov rsi, rax

    call __setup_kernel_entry
    cli
entry_halt:
    hlt
    jmp entry_halt

entry_gdt64:
.gdt_null: equ $ - entry_gdt64
    dw 0
    dw 0
    db 0
    db 0
    db 0
    db 0
.gdt_code: equ $ - entry_gdt64
    dw 0
    dw 0
    db 0
    db 0x9A
    db 0x20
    db 0
.gdt_data: equ $ - entry_gdt64
    dw 0
    dw 0
    db 0
    db 0x92
    db 0
    db 0
.gdt_pointer:
    dw $ - entry_gdt64 - 1
    dq entry_gdt64
entry_idt64:
    dw 0
    dd 0
section .bss

align 0x1000
PML4T: ; 1 PML4 Table
    resb 4096
align 0x1000
PDPT: ; 1 Page Directory Pointer Table
    resb 4096
align 0x1000
PDT: ; 4 Page Directory Tables
    resb 16384
align 0x1000
PT: ; 2048 Page Tables - 4 GiB
    resb 8388608
align 16
times 8192 resb 0
reserved_stack_space:
