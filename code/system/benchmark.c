#include <benchmark.h>
#include <system.h>
#include <print.h>

void benchmark_test_main(struct parameters* params) {
    uint64_t start = current_time();
    for (uint64_t i = 0; i < 4000000000; i++) {
        if (i % 100000 == 0) {
            ((uint64_t*)(params->local_memory))[1] = i;
        }
    }
    uint64_t total = current_time() - start;
    ((uint64_t*)(params->local_memory))[0] = total;
}

void benchmark_test_setup(void* global_memory, uint8_t thread_count) {

}

void benchmark_test_finish(void* global_memory, void** local_memories, uint8_t thread_count) {
    for (int i = 0; i < thread_count; i++) {
        printf("Thread #%i took %ims\n", i, ((uint64_t*)(local_memories[i]))[0]);
    }
}

void benchmark_test_secondly(void* global_memory, void** local_memories, uint8_t thread_count) {
    for (int i = 0; i < thread_count; i++) {
        printf("Thread #%i status %i\n", i, ((uint64_t*)(local_memories[i]))[1]);
    }
}

struct benchmark benchmark_benchmarks[] = {
    {
        "Count to 4 billion",
        &benchmark_test_main,
        &benchmark_test_setup,
        &benchmark_test_finish,
        &benchmark_test_secondly,
        0,
        sizeof(uint64_t) * 2,
        BENCHMARK_BOTH
    },
    {
        "Run some test benchmark",
        &benchmark_test_main,
        &benchmark_test_setup,
        &benchmark_test_finish,
        &benchmark_test_secondly,
        0,
        sizeof(uint64_t) * 2,
        BENCHMARK_SINGLECORE
    },
    {
        "Another benchmark",
        &benchmark_test_main,
        &benchmark_test_setup,
        &benchmark_test_finish,
        &benchmark_test_secondly,
        0,
        sizeof(uint64_t) * 2,
        BENCHMARK_BOTH
    },
    {
        "And this is yet another benchmark",
        &benchmark_test_main,
        &benchmark_test_setup,
        &benchmark_test_finish,
        &benchmark_test_secondly,
        0,
        sizeof(uint64_t) * 2,
        BENCHMARK_MULTICORE
    }
};

struct benchmark* benchmark_get_benchmarks() {
    return (struct benchmark*)(&benchmark_benchmarks);
}

uint64_t benchmark_num_benchmarks() {
    return sizeof(benchmark_benchmarks) / sizeof(struct benchmark);
}
