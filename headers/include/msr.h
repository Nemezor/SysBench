#ifndef IO_CPU_MSR_H
#define IO_CPU_MSR_H

#include <stdint.h>

#define MSR_IA32_THERM_STATUS 0x19C
#define MSR_TEMPERATURE_TARGET 0x1A2
#define MSR_IA32_APIC_BASE 0x1B

extern void msr_write(uint64_t data, uint64_t address);
extern uint64_t msr_read(uint64_t address);

#endif
