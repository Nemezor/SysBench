#include <smp.h>
#include <memory.h>
#include <stdint.h>
#include <apic.h>
#include <print.h>
#include <utils.h>
#include <idt.h>
#include <system.h>
#include <trampoline.h>
#include <gdt.h>
#include <paging.h>
#include <main.h>
#include <scheduler.h>
#include <fpu.h>

#define SMP_STACK_SIZE 1024*1024

struct smp_cpu {
    struct smp_cpu* next;
    struct idt_table* idt;
    uint8_t id;
    uint8_t apic;
    uint32_t flags;
    uint32_t lint0;
    uint32_t lint1;
    uint32_t apic_base;
    char status;
};

struct smp_cpu* smp_list;
uint8_t* smp_id_list;
uint32_t smp_id_list_length;
uint8_t smp_bsp_id;

void smp_register(uint8_t processor_id, uint8_t apic_id, uint32_t flags) {
    void* mem = memory_reserve(sizeof(struct smp_cpu));

    struct smp_cpu* entry = smp_list;

    if (smp_list == 0) {
        smp_list = (struct smp_cpu*)mem;
        entry = smp_list;
    }else{
        struct smp_cpu* last_entry = smp_list;
        while (entry != 0) {
            last_entry = entry;
            entry = entry->next;
        }
        entry = (struct smp_cpu*)mem;
        last_entry->next = entry;
    }

    entry->next = 0;
    entry->id = processor_id;
    entry->apic = apic_id;
    entry->flags = flags;
    entry->lint0 = 0;
    entry->lint1 = 0;
    entry->apic_base = 0;
    entry->idt = 0;
    entry->status = SMP_STATUS_UNINITIALIZED;
}

void smp_add_nmi_internal(struct smp_cpu* cpu, uint16_t flags, char lint) {
    uint32_t nmi = 802; // NMI and vector 2
    if (flags & 2) {
        nmi |= 1 << 13;
    }
    if (flags & 8) {
        nmi |= 1 << 15;
    }
    if (lint) {
        cpu->lint1 = nmi;
    }else{
        cpu->lint0 = nmi;
    }
}

void smp_add_nmi(uint8_t apic_id, uint16_t flags, char lint) {
    struct smp_cpu* entry = smp_list;

    if (apic_id == 0xFF) {
        while (entry != 0) {
            smp_add_nmi_internal(entry, flags, lint);
            entry = entry->next;
        }
    }else{
        while (entry != 0) {
            if (entry->apic == apic_id) {
                break;
            }
            entry = entry->next;
        }
        if (entry == 0) {
            return;
        }
        smp_add_nmi_internal(entry, flags, lint);
    }
}

uint8_t smp_setup(uint8_t apic_id) {
    struct smp_cpu* entry = smp_list;

    while (entry != 0) {
        if (entry->apic == apic_id) {
            break;
        }
        entry = entry->next;
    }
    if (entry == 0) {
        return 0;
    }
    uint8_t id = entry->id;
    printf("[[SMP ]: Enabling APIC for processor %i\n", id);
    entry->apic_base = apic_get_base();
    if (entry->lint0 != 0) {
        printf("[[SMP ]: Setting LINT0 as Non-Maskable Interrupt for processor %i\n", id);
        apic_write_lint(0, entry->apic_base, entry->lint0);
    }
    if (entry->lint1 != 0) {
        printf("[[SMP ]: Setting LINT1 as Non-Maskable Interrupt for processor %i\n", id);
        apic_write_lint(1, entry->apic_base, entry->lint1);
    }
    printf("[[SMP ]: Creating Interrupt Descriptor Table for processor %i\n", id);
    entry->idt = idt_create(entry->apic_base, entry->id);
    apic_write_spurious(entry->apic_base, 0x1FF);
    printf("[[SMP ]: Enabling interrupts for processor %i\n", id);
    sti();
    printf("[[SMP ]: Processor %i running\n", id);
    entry->status = SMP_STATUS_RUNNING;
    return entry->id;
}

void smp_finalize() {
    struct smp_cpu* entry = smp_list;
    uint32_t counter = 0;

    while (entry != 0) {
        counter++;
        entry = entry->next;
    }
    printf("[[SMP ]: Detected %i processors\n", counter);

    smp_id_list_length = counter;
    smp_id_list = (uint8_t*)memory_reserve(counter * sizeof(uint8_t));
    entry = smp_list;
    counter = 0;

    while (entry != 0) {
        smp_id_list[counter] = entry->id;
        entry = entry->next;
        counter++;
    }
    printf("[[SMP ]: Copying trampoline code to %X\n", TRAMPOLINE_BASE);
    trampoline_copy();
}

uint8_t* smp_ids() {
    return smp_id_list;
}

uint32_t smp_count() {
    return smp_id_list_length;
}

uint32_t smp_get_apic_base() {
    uint64_t idtr = idt_get_idtr();
    struct idt_table* idt = (struct idt_table*)idtr;
    return idt->apic_base;
}

char smp_boot(uint8_t processor_id) {
    struct smp_cpu* entry = smp_list;

    while (entry != 0) {
        if (entry->id == processor_id) {
            break;
        }
        entry = entry->next;
    }
    if (entry == 0 || entry->status != SMP_STATUS_UNINITIALIZED) {
        return SMP_STATUS_NONEXISTANT;
    }
    printf("[[SMP ]: Clearing trampoline\n");
    trampoline_clear();
    trampoline_set_gdtr(gdt_read_size(), gdt_read_ptr());
    trampoline_set_pml4(paging_read_pml4());
    trampoline_set_apic(entry->apic);
    trampoline_set_stack((uint64_t)memory_reserve(SMP_STACK_SIZE) + SMP_STACK_SIZE);
    uint8_t trampoline_page = 1;

    uint32_t apic_base = smp_get_apic_base();
    printf("[[SMP ]: Starting processor %i\n", entry->id);

    apic_iipi(apic_base, entry->apic);
    printf("[[SMP ]: Sent init IPI to processor %i\n", entry->id);
    sleep(10);
    apic_sipi(apic_base, entry->apic, trampoline_page);
    printf("[[SMP ]: Sent startup IPI to processor %i\n", entry->id);
    sleep(1);
    if (!trampoline_booted()) {
        printf("[[SMP ]: processor %i didn't start, waiting 1 second\n", entry->id);
        apic_sipi(apic_base, entry->apic, trampoline_page);
        printf("[[SMP ]: Sent second startup IPI to processor %i\n", entry->id);
        sleep(1000);
        if (!trampoline_booted()) {
            printf("[[SMP ]: processor %i didn't start after another SIPI, aborting\n", entry->id);
            entry->status = SMP_STATUS_FAILED;
            return entry->status;
        }
    }
    printf("[[SMP ]: Processor %i set startup signal\n", entry->id);
    entry->status = SMP_STATUS_BOOTING;

    trampoline_continue();
    while (entry->status == SMP_STATUS_BOOTING) {
        __asm__("nop");
    }
    return entry->status;
}

void smp_ap_entry() {
    trampoline_set_booted();

    while (!trampoline_can_continue()) {
        __asm__("nop");
    }

    volatile uint8_t* apic = (volatile uint8_t*)TRAMPOLINE_APIC;
    uint8_t apic_id = *apic;

    fpu_init();
    uint8_t core_id = smp_setup(apic_id);

    while (1) {
        halt();

        scheduler_run(core_id);
    }
}

void smp_set_bsp(uint8_t apic_id) {
    struct smp_cpu* entry = smp_list;

    while (entry != 0) {
        if (entry->apic == apic_id) {
            break;
        }
        entry = entry->next;
    }
    if (entry == 0) {
        return;
    }
    smp_bsp_id = entry->id;
}

char smp_is_bsp(uint8_t id) {
    return smp_bsp_id == id;
}

void smp_wake(uint8_t current_id, uint8_t target_id) {
    struct smp_cpu* entry = smp_list;
    struct smp_cpu* current;
    struct smp_cpu* target;

    while (entry != 0) {
        if (entry->id == current_id) {
            current = entry;
        }
        if (entry->id == target_id) {
            target = entry;
        }
        entry = entry->next;
    }
    if (target == current || target == 0 || current == 0) {
        return;
    }
    apic_ipi(current->apic_base, target->apic, SMP_WAKE_INTERRUPT);
}
