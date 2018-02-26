#ifndef UTILS_CPUID_H
#define UTILS_CPUID_H

#include <stdint.h>

struct cpuid {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
};

extern void cpuid_query(struct cpuid* cpuid_struct);

#endif
