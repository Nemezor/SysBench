#ifndef INTERRUPT_APIC_APIC_H
#define INTERRUPT_APIC_APIC_H

#include <stdint.h>
#include <acpi/tables.h>

void apic_initialize(struct acpi_MADT* madt_ptr);
uint8_t apic_enable();
void apic_eoi(uint32_t apic_base);
void apic_write_lint(char lint, uint32_t apic_base, uint32_t data);
void apic_write_spurious(uint32_t apic_base, uint32_t data);
uint32_t apic_get_base();
void apic_iipi(uint32_t apic_base, uint8_t apic_id);
void apic_sipi(uint32_t apic_base, uint8_t apic_id, uint8_t page_number);
void apic_ipi(uint32_t apic_base, uint8_t apic_id, uint8_t interrupt);

#endif
