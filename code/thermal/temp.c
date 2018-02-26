#include <temp.h>
#include <cpuid.h>
#include <print.h>
#include <stdint.h>
#include <utils.h>
#include <msr.h>
#include <intel/tcc.h>

char temp_intel = 0;
char temp_intel_legacy = 0;
char temp_intel_legacy_tcc = 0;
char temp_amd = 0;

char temp_supported() {
    printf("[[Temp]: Detecting CPU Vendor\n");
    struct cpuid regs;

    regs.rax = 0;
    regs.rbx = 0;
    regs.rcx = 0;
    regs.rdx = 0;

    cpuid_query(&regs);

    uint32_t vendor[4];
    vendor[0] = (uint32_t)regs.rbx;
    vendor[1] = (uint32_t)regs.rdx;
    vendor[2] = (uint32_t)regs.rcx;
    vendor[3] = (uint32_t)'\0';

    printf("[[Temp]: CPU Vendor: %s\n", &vendor);

    // GenuineIntel
    if (regs.rbx == 0x756E6547 && regs.rcx == 0x6C65746E && regs.rdx == 0x49656E69) {
        temp_intel = 1;

        regs.rax = 0x6;
        regs.rbx = 0;
        regs.rcx = 0;
        regs.rdx = 0;

        cpuid_query(&regs);

        if (regs.rax & 1) {
            printf("[[Temp]: Intel Digital Thermal Sensor Supported\n");
            return 1;
        }
    // AuthenticAMD
    }else if (regs.rbx == 0x68747541 && regs.rcx == 0x444D4163 && regs.rdx == 0x69746E65) {
        temp_amd = 1;

        regs.rax = 0x80000000;
        regs.rbx = 0;
        regs.rcx = 0;
        regs.rdx = 0;

        cpuid_query(&regs);

        if (regs.rax >= 0x80000007) {
            regs.rax = 0x80000007;
            regs.rbx = 0;
            regs.rcx = 0;
            regs.rdx = 0;

            cpuid_query(&regs);

            if (regs.rdx & 1) {
                printf("[[Temp]: AMD Digital Thermal Sensor Supported\n");
                return 1;
            }
        }
    }

    printf("[[Temp]: Couldn't find any supported temperature sensors\n");
    temp_intel = 0;
    temp_amd = 0;
    return 0;
}

char get_temperature() {
    if (temp_intel) {
        uint64_t status = msr_read(MSR_IA32_THERM_STATUS);
        uint8_t tcc = (uint8_t)((msr_read(MSR_TEMPERATURE_TARGET) >> 16) & 0xFF);

        if (temp_intel_legacy) {
            tcc = temp_intel_legacy_tcc;
        }else if (tcc == 0) {
            struct cpuid regs;

            regs.rax = 0x80000000;
            regs.rbx = 0;
            regs.rcx = 0;
            regs.rdx = 0;

            cpuid_query(&regs);

            if (regs.rax < 0x80000004) {
                temp_intel = 0;
                temp_amd = 0;
                return -1;
            }
            temp_intel_legacy = 1;
            uint32_t brand_string_l[12];
            char* brand_string = (char*)&brand_string_l;

            for (int i = 0, j = 0; i < 12; i += 4, j++) {
                regs.rax = 0x80000002 + j;
                regs.rbx = 0;
                regs.rcx = 0;
                regs.rdx = 0;

                cpuid_query(&regs);

                brand_string_l[i    ] = (uint32_t)regs.rax;
                brand_string_l[i + 1] = (uint32_t)regs.rbx;
                brand_string_l[i + 2] = (uint32_t)regs.rcx;
                brand_string_l[i + 3] = (uint32_t)regs.rdx;
            }

            tcc = intel_tcc_get(brand_string);
            temp_intel_legacy_tcc = tcc;
        }

        uint8_t resolution = (uint8_t)((status >> 27) & 0xF);
        uint8_t temperature = (uint8_t)((status >> 16) & 0x7F);
        char valid = (char)((status >> 31) & 1);

        if (valid) {
            return (char)(tcc - temperature);
        }else{
            return -1;
        }
    }else if (temp_amd) {
        
    }
    return -1;
}
