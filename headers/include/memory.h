#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#include <stdint.h>

extern uint32_t __lnk_kernel_end;

void memory_init();
void* memory_reserve(uint64_t size);
void* memory_alloc_benchmark(uint64_t size);
void memory_free_benchmark();
extern void memory_clear(uint64_t start, uint64_t size);

void* malloc(uint64_t size);
void free(void* memory);
void memcpy(void* a, void* b, uint64_t size);
void memset(void* mem, char c, uint64_t size);

#endif
