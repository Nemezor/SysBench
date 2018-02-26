#include <acpi.h>
#include <acpi/tables.h>
#include <apic.h>
#include <stdint.h>
#include <print.h>
#include <acpi/aml/interpreter.h>

#define ACPI_EBDA_ADDRESS 0x40E

#define ACPI_SIGNATURE_MADT "APIC"
#define ACPI_SIGNATURE_FADT "FACP"
#define ACPI_SIGNATURE_DSDT "DSDT"
#define ACPI_SIGNATURE_SSDT "SSDT"

struct acpi_RSDP1 {
    char signature[8];
    uint8_t checksum;
    char OEM_ID[6];
    uint8_t revision;
    uint32_t rsdt_ptr;
} __attribute__((packed));

struct acpi_RSDP2 {
    struct acpi_RSDP1 rsdp;
    uint32_t length;
    uint64_t xsdt_ptr;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

char acpi_64bit = 0;

char acpi_validate_table(void* table, uint32_t size) {
    uint64_t sum = 0;
    void* end = table + (uint64_t)size;
    for (; table < end; table++) {
        sum += *((char*)table);
    }
    return (sum & 0xFF) == 0;
}

void* acpi_get_xsdt(uint64_t i) {
    struct acpi_RSDP2* rsdp = (struct acpi_RSDP2*)i;

    printf("[[ACPI]: Found RSDP at %X\n", i);

    if (rsdp->rsdp.revision > 0) {
        printf("[[ACPI]: RSDP Version 2.0+\n");
        if (acpi_validate_table((void*)i, rsdp->length)) {
            printf("[[ACPI]: RSDP Table Valid (Using XSDT)\n");
            acpi_64bit = 1;
            return (void*)(rsdp->xsdt_ptr);
        }
    }else{
        printf("[[ACPI]: RSDP Version 1.0\n");
        if (acpi_validate_table((void*)i, sizeof(struct acpi_RSDP1))) {
            printf("[[ACPI]: RSDP Table Valid (Using RSDT)\n");
            acpi_64bit = 0;
            return (void*)((uint64_t)rsdp->rsdp.rsdt_ptr);
        }
    }
    return 0;
}

char acpi_validate_and_print(char* name, void* table) {
    printf("[[ACPI]: Found %s\n", name);
    struct acpi_header* header = (struct acpi_header*)table;

    if (!acpi_validate_table(table, header->length)) {
        printf("[[ACPI]: %s Invalid!\n", name);
        return 0;
    }
    printf("[[ACPI]: %s Valid\n", name);
    return 1;
}

uint32_t acpi_signature_to_int(char* name) {
    uint32_t sig = 0;
    sig |= ((uint32_t)name[0]) << 0;
    sig |= ((uint32_t)name[1]) << 8;
    sig |= ((uint32_t)name[2]) << 16;
    sig |= ((uint32_t)name[3]) << 24;
    return sig;
}

char acpi_scan_tables(void* xsdt_ptr) {
    struct acpi_header* xsdt = (struct acpi_header*)xsdt_ptr;

    uint64_t length = (uint64_t)xsdt->length;
    if (!acpi_validate_table(xsdt_ptr, length)) {
        printf("[[ACPI]: %s Invalid!\n", acpi_64bit ? "XSDT" : "RSDT");
        return 0;
    }
    printf("[[ACPI]: %s Valid\n", acpi_64bit ? "XSDT" : "RSDT");

    for (uint64_t i = (uint64_t)(xsdt_ptr + sizeof(struct acpi_header)); i < (uint64_t)xsdt_ptr + length; i += (acpi_64bit ? 8 : 4)) {
        void* table_ptr = (void*)(*((uint64_t*)i) & (acpi_64bit ? 0xFFFFFFFFFFFFFFFF : 0xFFFFFFFF));

        if (*((uint32_t*)table_ptr) == acpi_signature_to_int(ACPI_SIGNATURE_MADT)) {
            if (acpi_validate_and_print("MADT", table_ptr)) {
                struct acpi_MADT* madt = (struct acpi_MADT*)table_ptr;
                apic_initialize(madt);
            }
        }else if (*((uint32_t*)table_ptr) == acpi_signature_to_int(ACPI_SIGNATURE_FADT)) {
            if (acpi_validate_and_print("FADT", table_ptr)) {
                struct acpi_FADT* fadt = (struct acpi_FADT*)table_ptr;
                struct acpi_header* dsdt = acpi_64bit ? ((struct acpi_header*)fadt->X_dsdt) : ((struct acpi_header*)(uint64_t)fadt->dsdt);
                if (acpi_validate_and_print("DSDT", (void*)dsdt)) {
                    aml_process(dsdt);
                }
            }
        }else if (*((uint32_t*)table_ptr) == acpi_signature_to_int(ACPI_SIGNATURE_SSDT)) {
            if (acpi_validate_and_print("SSDT", table_ptr)) {
                struct acpi_header* ssdt = (struct acpi_header*)table_ptr;
                aml_process(ssdt);
            }
        }
    }
    return 1;
}

char acpi_initialize() {
    uint32_t ebda_base = ((uint32_t)(*((uint16_t*)ACPI_EBDA_ADDRESS))) << 4;

    printf("[[ACPI]: EBDA Base Address: %X\n", ebda_base);

    void* xsdt = 0;
    uint64_t magic = 0;

    for (uint32_t i = ebda_base; i < ebda_base + 1024; i += 16) {
        magic = *((uint64_t*)((uint64_t)i));

        if (magic == 0x2052545020445352) {
            xsdt = acpi_get_xsdt(i);
            if (xsdt != 0) {
                break;
            }
        }
    }
    if (xsdt == 0) {
        for (uint64_t i = 0xE0000; i < 0xFFFFF; i += 16) {
            magic = *((uint64_t*)i);

            if (magic == 0x2052545020445352) {
                xsdt = acpi_get_xsdt(i);
                if (xsdt != 0) {
                    break;
                }
            }
        }
    }
    if (xsdt == 0) {
        printf("[[ACPI]: Failed to find ACPI tables!\n");
        return 0;
    }
    return acpi_scan_tables(xsdt);
}

char acpi_is_64bit() {
    return acpi_64bit;
}
