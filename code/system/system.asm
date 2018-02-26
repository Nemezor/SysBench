[BITS 64]

global system_call

; void system_call(char number, uint32_t code);
system_call:
    mov [.intnum], dil
    db 0xCD
    .intnum: db 0x00
    ret
