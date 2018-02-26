[BITS 64]

global stop
global halt
global cli
global sti
global brk

; void stop();
stop:
    .loop:
    hlt
    jmp .loop
    ret

; void halt();
halt:
    hlt
    ret

; void sti();
sti:
    sti
    ret

; void cli();
cli:
    cli
    ret

; void brk();
brk:
    xchg bx, bx
    ret
