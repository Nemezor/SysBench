[BITS 64]

global paging_read_pml4

; uint64_t paging_read_pml4();
paging_read_pml4:
    mov rax, cr3
    mov rcx, 0xFFF
    not rcx
    and rax, rcx
    ret
