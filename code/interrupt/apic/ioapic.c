#include <ioapic.h>
#include <memory.h>

struct ioapic_entry {
    struct ioapic_entry* next;
    char id;
    uint32_t base;
    uint32_t int_base;
    uint8_t max_redirections;
};

struct ioapic_entry* ioapic_list;

uint32_t ioapic_read(uint64_t base, uint32_t reg) {
    volatile uint32_t* apic = (volatile uint32_t*)base;
    apic[0] = reg & 0xFF;
    return apic[4];
}

void ioapic_write(uint64_t base, uint32_t reg, uint32_t data) {
    volatile uint32_t* apic = (volatile uint32_t*)base;
    apic[0] = reg & 0xFF;
    apic[4] = data;
}

void ioapic_init(char id, uint32_t base, uint32_t interrupt_base) {
    void* mem = memory_reserve(sizeof(struct ioapic_entry));

    struct ioapic_entry* entry = ioapic_list;

    if (ioapic_list == 0) {
        ioapic_list = (struct ioapic_entry*)mem;
        entry = ioapic_list;
    }else{
        struct ioapic_entry* last_entry = ioapic_list;
        while (entry != 0) {
            last_entry = entry;
            entry = entry->next;
        }
        entry = (struct ioapic_entry*)mem;
        last_entry->next = entry;
    }

    entry->next = 0;
    entry->id = id;
    entry->base = base;
    entry->int_base = interrupt_base;
    entry->max_redirections = (char)(ioapic_read(base, 1) & 0xFF);
}

void ioapic_redirect(char bus, char irq, uint32_t interrupt, uint16_t flags, uint8_t bsp_apic_id) {
    struct ioapic_entry* entry = ioapic_list;

    while (entry != 0) {
        if (entry->int_base <= interrupt) {
            break;
        }
        entry = entry->next;
    }
    if (entry == 0) {
        return;
    }
    uint64_t redirection = irq + 48;
    if (flags & 2) {
        redirection |= 1 << 13;
    }
    if (flags & 8) {
        redirection |= 1 << 15;
    }
    redirection |= ((uint64_t)bsp_apic_id) << 56;

    uint32_t redirection_reg = interrupt * 2 + 0x10;
    ioapic_write(entry->base, redirection_reg, (uint32_t)redirection);
    redirection_reg++;
    ioapic_write(entry->base, redirection_reg, (uint32_t)(redirection >> 32));
}
