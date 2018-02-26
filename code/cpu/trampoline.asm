[BITS 64]

; code = 4096 upwards (page #1)
; stack = 0x800 downwards
; gdtr = 0x600 - uint16_t + uint64_t
; APIC id = 0x620 - uint8_t
; booted = 0x630 - uint8_t
; continue = 0x640 - uint8_t
; PML4 = 0x650 - uint32_t (ptr)

%define RELOC_BASE 4096
%define STACK_PTR 0x800
%define GDTR 0x600
%define APIC 0x620
%define BOOT_FLG 0x630
%define CONT_FLG 0x640
%define PML4T_PTR 0x650
%define RELOC(x) (x + RELOC_BASE - trampoline_start)

global trampoline_copy
global trampoline_set_gdtr
global trampoline_set_apic
global trampoline_set_pml4
global trampoline_set_stack
global trampoline_clear

extern smp_ap_entry

; void trampoline_copy();
trampoline_copy:
    mov rdi, RELOC_BASE
    mov rsi, trampoline_start
    mov rcx, (trampoline_end - trampoline_start + 1)
    cld
    rep movsb
    ret

; void trampoline_set_gdtr(uint16_t length, uint64_t pointer);
trampoline_set_gdtr:
    mov rax, GDTR
    mov [rax], di
    mov [rax + 2], rsi
    ret

; void trampoline_set_apic(uint8_t apic_id);
trampoline_set_apic:
    mov rax, APIC
    mov [rax], dil
    ret

; void trampoline_set_pml4(uint32_t pointer);
trampoline_set_pml4:
    mov rax, PML4T_PTR
    mov [rax], edi
    ret

; void trampoline_set_stack(uint64_t pointer);
trampoline_set_stack:
    mov rax, STACK_PTR
    mov [rax], rdi
    ret

; void trampoline_clear();
trampoline_clear:
    mov rax, BOOT_FLG
    mov byte [rax], 0
    mov rax, CONT_FLG
    mov byte [rax], 0
    ret

; called by AP on startup
[BITS 16]
trampoline_start:
    xor ax, ax
    xor bx, bx
    xor cx, cx
    xor dx, dx
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov sp, RELOC_BASE
    mov bp, sp

    mov eax, [PML4T_PTR]
    mov cr3, eax
    mov eax, cr4
    or eax, 0xA0
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x00000100
    wrmsr

    mov eax, 0x80000001
    mov cr0, eax

    lgdt [GDTR]
    jmp 0x08:RELOC(.long_mode)
[BITS 64]
.long_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, [STACK_PTR]
    mov rbp, rsp

    mov rax, smp_ap_entry
    call rax
trampoline_end:
