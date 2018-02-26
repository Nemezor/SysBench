#include <idt.h>
#include <isr.h>
#include <stdint.h>
#include <print.h>
#include <memory.h>

void idt_set_entry(struct idt_table* idt, void* function, uint32_t index) {
    uint64_t* part1 = &idt->idt_map[index * 2];
    uint64_t* part2 = &idt->idt_map[index * 2 + 1];
    *part1 = 0;
    *part2 = 0;

    *part1 |= ((uint64_t)function) & 0xFFFF;
    *part1 |= 0x08ULL << 16;
    *part1 |= 0x8EULL << 40;
    *part1 |= (((uint64_t)function) & 0xFFFF0000) << 32;
    *part2 |= (((uint64_t)function) & 0xFFFFFFFF00000000) >> 32;
}

void idt_install(struct idt_table* idt) {
    for (int i = 0; i < 512; i++) {
        idt->idt_map[i] = 0;
    }
    printf("[[IDT ]: Installing Interrupt Map\n");
    isr_install(idt);
    idt_load((uint64_t)((void*)(&idt->idt_map)), 512 * sizeof(uint64_t) - 1);
}

struct idt_table* idt_create(uint32_t apic_base, uint8_t id) {
    struct idt_table* idt = memory_reserve(sizeof(struct idt_table));

    idt->apic_base = apic_base;
    idt->processor_id = id;
    idt_install(idt);
    return idt;
}
