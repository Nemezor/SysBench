#include <apic.h>
#include <stdint.h>
#include <print.h>
#include <msr.h>
#include <utils.h>
#include <ioapic.h>
#include <smp.h>
#include <cpuid.h>
#include <acpi/tables.h>

#define APIC_REGISTER_SPURIOUS 0xF0
#define APIC_REGISTER_EOI 0xB0
#define APIC_REGISTER_LINT0 0x350
#define APIC_REGISTER_LINT1 0x360
#define APIC_REGISTER_ICR0 0x300
#define APIC_REGISTER_ICR1 0x310

struct apic_record {
    char type;
    char length;
} __attribute__((packed));

struct apic_LAPIC {
    uint8_t processor_ID;
    uint8_t APIC_ID;
    uint32_t flags;
} __attribute__((packed));

struct apic_IOAPIC {
    uint8_t IOAPIC_ID;
    uint8_t reserved;
    uint32_t IOAPIC_address;
    uint32_t interrupt_base;
} __attribute__((packed));

struct apic_ISO {
    uint8_t bus;
    uint8_t irq;
    uint32_t interrupt;
    uint16_t flags;
} __attribute__((packed));

struct apic_NMI {
    uint8_t processor_ID;
    uint16_t flags;
    uint8_t LINT;
} __attribute__((packed));

struct acpi_MADT* apic_madt = 0;

uint32_t apic_read(uint64_t apic_base, uint64_t reg) {
    return *((uint32_t*)(void*)(apic_base + reg));
}

void apic_write(uint64_t apic_base, uint64_t reg, uint32_t data) {
    *((uint32_t*)(void*)(apic_base + reg)) = data;
}

void apic_initialize(struct acpi_MADT* madt_ptr) {
    apic_madt = madt_ptr;
    printf("[[APIC]: APIC Initialized\n");
}

void apic_eoi(uint32_t apic_base) {
    apic_write((uint64_t)apic_base, APIC_REGISTER_EOI, 0);
}

uint8_t apic_enable() {
    if (apic_madt == 0) {
        return 0;
    }
    struct apic_record* ptr = (struct apic_record*)(((void*)apic_madt) + sizeof(struct acpi_MADT));
    void* end = ((void*)apic_madt) + apic_madt->header.length;

    printf("[[APIC]: Reading APIC ID of the BSP\n");
    struct cpuid regs;

    regs.rax = 0xB;
    regs.rbx = 0;
    regs.rcx = 0;
    regs.rdx = 0;
    cpuid_query(&regs);
    uint8_t bsp_id = (uint8_t)regs.rdx & 0xF; // TODO fix?

    printf("[[APIC]: Searching for other processors\n");

    while (((void*)ptr) < end) {
        char type = ptr->type;
        char size = ptr->length;

        if (type == 0) {
            struct apic_LAPIC* lapic = (struct apic_LAPIC*)(((void*)ptr) + sizeof(struct apic_record));
            printf("[[APIC]: Found APIC %i of processor %i\n", lapic->APIC_ID, lapic->processor_ID);
            if (lapic->flags & 1) {
                smp_register(lapic->processor_ID, lapic->APIC_ID, lapic->flags);
            }else{
                printf("[[APIC]: Processor %i disabled\n", lapic->processor_ID);
            }
        }else if (type == 1) {
            struct apic_IOAPIC* ioapic = (struct apic_IOAPIC*)(((void*)ptr) + sizeof(struct apic_record));
            printf("[[APIC]: Found I/O APIC %i at %x for interrupts at %i\n", ioapic->IOAPIC_ID, ioapic->IOAPIC_address, ioapic->interrupt_base);
            ioapic_init(ioapic->IOAPIC_ID, ioapic->IOAPIC_address, ioapic->interrupt_base);
        }else if (type == 2) {
            struct apic_ISO* iso = (struct apic_ISO*)(((void*)ptr) + sizeof(struct apic_record));
            printf("[[APIC]: Found Interrupt Source Override for %i:%i for interrupt %i\n", iso->bus, iso->irq, iso->interrupt);
            ioapic_redirect(iso->bus, iso->irq, iso->interrupt, iso->flags, bsp_id);
        }else if (type == 4) {
            struct apic_NMI* nmi = (struct apic_NMI*)(((void*)ptr) + sizeof(struct apic_record));
            printf("[[APIC]: Found Non-Maskable Interrupt %s of processor %i\n", (nmi->LINT == 0) ? "LINT0" : "LINT1", nmi->processor_ID);
            smp_add_nmi(nmi->processor_ID, nmi->flags, nmi->LINT);
        }
        ptr = (struct apic_record*)(((void*)ptr) + size);
    }
    smp_finalize();

    printf("[[APIC]: Enabling BSP-LAPIC (id %i)\n", bsp_id);
    uint64_t msr = msr_read(MSR_IA32_APIC_BASE);
    msr_write(MSR_IA32_APIC_BASE, msr | (1 << 11));
    smp_set_bsp(bsp_id);
    return smp_setup(bsp_id);
}

void apic_write_lint(char lint, uint32_t apic_base, uint32_t data) {
    apic_write((uint64_t)apic_base, lint == 0 ? APIC_REGISTER_LINT0 : APIC_REGISTER_LINT1, data);
}

void apic_write_spurious(uint32_t apic_base, uint32_t data) {
    apic_write(apic_base, APIC_REGISTER_SPURIOUS, apic_read(apic_base, APIC_REGISTER_SPURIOUS) | data);
}

uint32_t apic_get_base() {
    //return (uint32_t)msr_read(MSR_IA32_APIC_BASE) & 0xFFFFF000;
    return 0xFEE00000; //TODO maybe some fancy msr stuff?
}

void apic_iipi(uint32_t apic_base, uint8_t apic_id) {
    apic_write(apic_base, APIC_REGISTER_ICR1, apic_id << 24);
    apic_write(apic_base, APIC_REGISTER_ICR0, 0x4500);
}

void apic_sipi(uint32_t apic_base, uint8_t apic_id, uint8_t page_number) {
    apic_write(apic_base, APIC_REGISTER_ICR1, apic_id << 24);
    apic_write(apic_base, APIC_REGISTER_ICR0, 0x4600 | page_number);
}

void apic_ipi(uint32_t apic_base, uint8_t apic_id, uint8_t interrupt) {
    apic_write(apic_base, APIC_REGISTER_ICR1, apic_id << 24);
    apic_write(apic_base, APIC_REGISTER_ICR0, 0x4000 | interrupt);
}
