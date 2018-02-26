#ifndef SYSTEM_BENCHMARK_H
#define SYSTEM_BENCHMARK_H

#include <stdint.h>
#include <stdbool.h>

#define BENCHMARK_SINGLECORE 1
#define BENCHMARK_MULTICORE 2
#define BENCHMARK_BOTH 3

struct parameters {
    uint8_t thread_id;
    bool main_thread;
    void* global_memory;
    void* local_memory;
};

struct benchmark {
    char name[50];
    void (*main)(struct parameters* params);
    void (*setup)(void* global_memory, uint8_t thread_count);
    void (*finish)(void* global_memory, void** local_memories, uint8_t thread_count);
    void (*secondly_update)(void* global_memory, void** local_memories, uint8_t thread_count);
    uint64_t global_memory_size;
    uint64_t local_memory_size;
    char type;
};

struct benchmark* benchmark_get_benchmarks();
uint64_t benchmark_num_benchmarks();

#endif
