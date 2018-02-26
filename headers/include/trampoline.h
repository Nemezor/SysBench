#ifndef CPU_TRAMPOLINE_H
#define CPU_TRAMPOLINE_H

#include <stdint.h>

#define TRAMPOLINE_BASE 4096
#define TRAMPOLINE_STACK 0x800
#define TRAMPOLINE_GDTR 0x600
#define TRAMPOLINE_APIC 0x620
#define TRAMPOLINE_BOOT 0x630
#define TRAMPOLINE_CONT 0x640
#define TRAMPOLINE_PML4 0x650

extern void trampoline_copy();
extern void trampoline_set_gdtr(uint16_t length, uint64_t pointer);
extern void trampoline_set_apic(uint8_t apic_id);
extern void trampoline_set_pml4(uint32_t pointer);
extern void trampoline_set_stack(uint64_t pointer);
extern void trampoline_clear();
void trampoline_continue();
char trampoline_booted();
char trampoline_can_continue();
void trampoline_set_booted();

#endif
