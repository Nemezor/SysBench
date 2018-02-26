#ifndef CPU_SMP_H
#define CPU_SMP_H

#include <stdint.h>

#define SMP_STATUS_UNINITIALIZED 0
#define SMP_STATUS_RUNNING 1
#define SMP_STATUS_FAILED 2
#define SMP_STATUS_BOOTING 3
#define SMP_STATUS_NONEXISTANT 4

#define SMP_WAKE_INTERRUPT 69

void smp_register(uint8_t processor_id, uint8_t apic_id, uint32_t flags);
void smp_add_nmi(uint8_t apic_id, uint16_t flags, char lint);
uint8_t smp_setup(uint8_t apic_id);
void smp_finalize();
uint8_t* smp_ids();
uint32_t smp_count();
char smp_boot(uint8_t processor_id);
void smp_ap_entry();
void smp_set_bsp(uint8_t apic_id);
char smp_is_bsp(uint8_t id);
void smp_wake(uint8_t current_id, uint8_t target_id);

#endif
