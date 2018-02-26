#include <memory.h>
#include <print.h>

#define MEMORY_USABLE_ALLOCATION 1024*1024*1024

void* memory_begin;
void* memory_end;
void* benchmark_begin;

void memory_init() {
    memory_begin = &__lnk_kernel_end;
    benchmark_begin = memory_begin;
    memory_end = memory_begin + MEMORY_USABLE_ALLOCATION;
    printf("[[mem ]: Usable memory from %P to %P\n", memory_begin, memory_end);
}

void* malloc(uint64_t size) {
    return memory_reserve(size);
}

void free(void* memory) {

}

void* memory_reserve(uint64_t size) {
    if (memory_begin + size > memory_end) {
        printf("[[mem ]: Out of memory!\n");
        //TODO stop the system (including SMP)
        return 0;
    }
    void* mem = memory_begin;
    memory_clear((uint64_t)mem, size);
    memory_begin += size;
    benchmark_begin = memory_begin;
    return mem;
}

void* memory_alloc_benchmark(uint64_t size) {
    if (benchmark_begin + size > memory_end) {
        printf("[[mem ]: Out of memory!\n");
        //TODO stop the system (including SMP)
        return 0;
    }
    void* mem = benchmark_begin;
    memory_clear((uint64_t)mem, size);
    benchmark_begin += size;
    return mem;
}

void memory_free_benchmark() {
    benchmark_begin = memory_begin;
}

void memcpy(void* a, void* b, uint64_t size) {
    char* ac = (char*)a;
    char* bc = (char*)b;
    for (uint64_t i = 0; i < size; i++) {
        bc[i] = ac[i];
    }
}

void memset(void* mem, char c, uint64_t size) {
    char* mc = (char*)mem;
    for (uint64_t i = 0; i < size; i++) {
        mc[i] = c;
    }
}
