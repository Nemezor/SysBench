[BITS 64]

global ports_out8
global ports_out16
global ports_out32
global ports_in8
global ports_in16
global ports_in32

ports_out8:
	mov al, sil
	mov dx, di
	out dx, al
	ret

ports_out16:
    mov ax, si
    mov dx, di
	out dx, ax
	ret

ports_out32:
    mov eax, esi
    mov dx, di
	out dx, eax
	ret

ports_in8:
    mov dx, di
    xor rax, rax
	in al, dx
	ret

ports_in16:
    mov dx, di
    xor rax, rax
	in ax, dx
	ret

ports_in32:
        mov dx, di
        xor rax, rax
	in eax, dx
	ret
