#ifndef INTERRUPT_APIC_IOAPIC_H
#define INTERRUPT_APIC_IOAPIC_H

#include <stdint.h>

void ioapic_init(char id, uint32_t base, uint32_t interrupt_base);
void ioapic_redirect(char bus, char irq, uint32_t interrupt, uint16_t flags, uint8_t bsp_apic_id);

#endif
