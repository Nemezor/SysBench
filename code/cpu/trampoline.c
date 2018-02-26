#include <trampoline.h>
#include <stdint.h>
#include <print.h>

void trampoline_continue() {
    volatile uint8_t* cont = (volatile uint8_t*)TRAMPOLINE_CONT;
    *cont = 1;
}

char trampoline_booted() {
    volatile uint8_t* boot = (volatile uint8_t*)TRAMPOLINE_BOOT;
    return *boot;
}

char trampoline_can_continue() {
    volatile uint8_t* cont = (volatile uint8_t*)TRAMPOLINE_CONT;
    return *cont;
}

void trampoline_set_booted() {
    volatile uint8_t* boot = (volatile uint8_t*)TRAMPOLINE_BOOT;
    *boot = 1;
}
