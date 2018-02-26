#ifndef INTERRUPT_IDT_IDT_H
#define INTERRUPT_IDT_IDT_H

#include <stdint.h>

struct idt_table {
    uint64_t idt_map[512];
    uint32_t apic_base;
    uint8_t processor_id;
};

struct idt_table* idt_create(uint32_t apic_base, uint8_t id);
void idt_set_entry(struct idt_table* idt, void* function, uint32_t index);
extern void idt_load(uint64_t base, int32_t size);
extern uint64_t idt_get_idtr();

#endif
