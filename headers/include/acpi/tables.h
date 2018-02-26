#ifndef ACPI_TABLES_H
#define ACPI_TABLES_H

#include <stdint.h>

#define ACPI_GAS_MEMORY 0
#define ACPI_GAS_IO 1
#define ACPI_GAS_PCI 2
#define ACPI_GAS_EMBED 3
#define ACPI_GAS_SMBUS 4
#define ACPI_GAS_FIXHW 0x7F

#define ACPI_GAS_AUNDEF 0
#define ACPI_GAS_ABYTE 1
#define ACPI_GAS_AWORD 2
#define ACPI_GAS_ADWORD 3
#define ACPI_GAS_AQWORD 4

struct acpi_gas {
    uint8_t address_space;
    uint8_t bit_width;
    uint8_t bit_offset;
    uint8_t access_size;
    uint64_t address;
} __attribute__((packed));

struct acpi_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OEM_ID[6];
    char OEM_table_ID[8];
    uint32_t OEM_revision;
    uint32_t creator_ID;
    uint32_t creator_revision;
} __attribute__((packed));

struct acpi_MADT {
    struct acpi_header header;
    uint32_t controller_address;
    uint32_t flags;
} __attribute__((packed));

struct acpi_FADT {
    struct acpi_header header;
    uint32_t firmware_ctrl;
    uint32_t dsdt;
    uint8_t reserved;
    uint8_t preferred_power_management_profile;
    uint16_t SCI_interrupt;
    uint32_t SMI_command_port;
    uint8_t ACPI_enable;
    uint8_t ACPI_disable;
    uint8_t S4BIOS_REQ;
    uint8_t PSTATE_control;
    uint32_t PM1a_event_block;
    uint32_t PM1b_event_block;
    uint32_t PM1a_control_block;
    uint32_t PM1b_control_block;
    uint32_t PM2_control_block;
    uint32_t PM_timer_block;
    uint32_t GPE0_block;
    uint32_t GPE1_block;
    uint8_t PM1_event_length;
    uint8_t PM1_control_length;
    uint8_t PM2_control_length;
    uint8_t PM_timer_length;
    uint8_t GPE0_length;
    uint8_t GPE1_length;
    uint8_t GPE1_base;
    uint8_t CSTATE_control;
    uint16_t worst_C2_latency;
    uint16_t worst_C3_latency;
    uint16_t flush_size;
    uint16_t flush_stride;
    uint8_t duty_offset;
    uint8_t duty_width;
    uint8_t day_alarm;
    uint8_t month_alarm;
    uint8_t century;
    uint16_t boot_architecture_flags;
    uint8_t reserved2;
    uint32_t flags;
    struct acpi_gas reset_reg;
    uint8_t reset_value;
    uint8_t reserved3[3];
    uint64_t X_firmware_control;
    uint64_t X_dsdt;
    struct acpi_gas X_PM1a_event_block;
    struct acpi_gas X_PM1b_event_block;
    struct acpi_gas X_PM1a_control_block;
    struct acpi_gas X_PM1b_control_block;
    struct acpi_gas X_PM2_control_block;
    struct acpi_gas X_PM_timer_block;
    struct acpi_gas X_GPE0_block;
    struct acpi_gas X_GPE1_block;
} __attribute__((packed));

#endif
