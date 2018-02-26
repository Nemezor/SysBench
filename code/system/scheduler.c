#include <scheduler.h>
#include <stdint.h>
#include <print.h>
#include <memory.h>
#include <stdbool.h>
#include <system.h>
#include <smp.h>

struct scheduler_worker {
    struct scheduler_worker* next;
    uint8_t id;
    bool running;
    bool gfx;
    uint32_t gfx_count;
    uint32_t gfx_offset;
    void (*gfx_load)(uint32_t id, void* uniform_data);
    void* gfx_data;
    struct scheduler_task task;
};

struct scheduler_worker* scheduler_workers;
char scheduler_can_run = 0;
void* scheduler_current_global_memory;
void** scheduler_current_local_memories;
uint8_t scheduler_current_thread_count;
void (*scheduler_current_secondly_callback)(void* global_memory, void** local_memories, uint8_t thread_count);

void scheduler_add_worker(uint8_t id) {
    printf("[[Schd]: Registering worker thread %i\n", id);
    struct scheduler_worker* worker = memory_reserve(sizeof(struct scheduler_worker));

    worker->id = id;
    worker->task.main = 0;
    worker->task.params = 0;
    worker->next = 0;
    worker->running = false;
    worker->gfx = false;
    worker->gfx_count = 0;
    worker->gfx_offset = 0;
    worker->gfx_load = 0;

    if (scheduler_workers == 0) {
        scheduler_workers = worker;
    }else{
        struct scheduler_worker* entry = scheduler_workers;
        while (entry->next != 0) {
            entry = entry->next;
        }
        entry->next = worker;
    }
}

struct scheduler_worker* scheduler_find_worker(uint8_t id) {
    struct scheduler_worker* worker = scheduler_workers;

    while (worker != 0) {
        if (worker->id == id) {
            return worker;
        }
        worker = worker->next;
    }
    return 0;
}

void scheduler_run(uint8_t id) {
    struct scheduler_worker* worker = scheduler_find_worker(id);
    if (worker->gfx) {
        if (scheduler_can_run && worker != 0 && worker->gfx_load != 0) {
            worker->running = true;
            for (uint32_t gid = worker->gfx_offset; gid < worker->gfx_offset + worker->gfx_count; gid++) {
                worker->gfx_load(gid, worker->gfx_data);
            }
            worker->running = false;
            worker->gfx_load = 0;
            worker->gfx_offset = 0;
            worker->gfx_count = 0;
            worker->gfx = false;
        }
    }else{
        if (scheduler_can_run && worker != 0 && worker->task.main != 0 && worker->task.params != 0) {
            worker->running = true;
            worker->task.main(worker->task.params);
            worker->running = false;
            worker->task.main = 0;
            worker->task.params = 0;
        }
    }
}

void scheduler_synchronize() {
    while (true) {
        bool some_running = false;
        struct scheduler_worker* worker = scheduler_workers;

        while (worker != 0) {
            if (worker->running) {
                some_running = true;
                break;
            }
            worker = worker->next;
        }
        if (!some_running) {
            break;
        }
        sleep(1);
    }
}

void scheduler_secondly_callback() {
    if (scheduler_current_secondly_callback != 0) {
        scheduler_current_secondly_callback(scheduler_current_global_memory, scheduler_current_local_memories, scheduler_current_thread_count);
    }
}

void scheduler_start(struct benchmark* bench, uint8_t bsp_id, char multicore) {
    printf("[[Schd]: Preparing benchmark: '%s (%s)'\n", bench->name, multicore ? "Multicore" : "Singlecore");
    struct scheduler_worker* worker = scheduler_workers;
    uint8_t counter = 0;

    if (multicore) {
        while (worker != 0) {
            worker = worker->next;
            counter++;
        }
    }else{
        counter = 1;
    }
    void* global_memory = 0;
    if (bench->global_memory_size > 0) {
        printf("[[Schd]: Allocating %i bytes of global memory\n", bench->global_memory_size);
        global_memory = memory_alloc_benchmark(bench->global_memory_size);
    }
    bench->setup(global_memory, counter);

    void* params_memory = memory_alloc_benchmark(counter * sizeof(struct parameters));
    void* local_memory = 0;
    if (bench->local_memory_size > 0) {
        printf("[[Schd]: Allocating %i bytes of local memory\n", counter * bench->local_memory_size);
        local_memory = memory_alloc_benchmark(counter * bench->local_memory_size);
    }
    void** local_memories = (void**)memory_alloc_benchmark(counter * sizeof(void*));
    worker = scheduler_workers;
    uint64_t offset = 0;

    if (multicore) {
        while (worker != 0) {
            worker->task.main = bench->main;
            worker->task.params = (struct parameters*)(params_memory + (offset * sizeof(struct parameters)));
            worker->task.params->thread_id = offset;
            worker->task.params->global_memory = global_memory;
            worker->task.params->local_memory = local_memory + offset * bench->local_memory_size;
            if (worker->id == bsp_id) {
                worker->task.params->main_thread = true;
            }else{
                worker->task.params->main_thread = false;
            }
            local_memories[offset] = worker->task.params->local_memory;
            worker = worker->next;
            offset++;
        }
    }else{
        worker = scheduler_find_worker(bsp_id);
        worker->task.main = bench->main;
        worker->task.params = (struct parameters*)params_memory;
        worker->task.params->thread_id = 0;
        worker->task.params->global_memory = global_memory;
        worker->task.params->local_memory = local_memory;
        worker->task.params->main_thread = true;
        local_memories[0] = worker->task.params->local_memory;
    }
    scheduler_current_global_memory = global_memory;
    scheduler_current_local_memories = local_memories;
    scheduler_current_thread_count = counter;
    scheduler_current_secondly_callback = bench->secondly_update;

    printf("[[Schd]: Running benchmark: '%s (%s)'\n", bench->name, multicore ? "Multicore" : "Singlecore");

    scheduler_can_run = 1;
    if (multicore) {
        worker = scheduler_workers;

        while (worker != 0) {
            if (worker->id != bsp_id) {
                worker->running = true;
                smp_wake(bsp_id, worker->id);
            }
            worker = worker->next;
        }
    }

    scheduler_run(bsp_id);
    scheduler_synchronize();

    printf("[[Schd]: Cleaning up benchmark: '%s (%s)'\n", bench->name, multicore ? "Multicore" : "Singlecore");

    scheduler_can_run = 0;
    bench->finish(global_memory, local_memories, counter);
    memory_free_benchmark();
}

void scheduler_dispatch_gfx(uint8_t bsp_id, uint32_t count, void (*shader)(uint32_t id, void* uniform_data), void* uniform_data) {
    struct scheduler_worker* worker = scheduler_workers;
    uint32_t cores = 0;

    while (worker != 0) {
        worker = worker->next;
        cores++;
    }

    if (count == 0) {
        return;
    }
    if (count == 1) {
        worker = scheduler_workers;
        while (worker != 0) {
            if (worker->id == bsp_id) {
                worker->gfx = true;
                worker->gfx_load = shader;
                worker->gfx_count = 1;
                worker->gfx_offset = 0;
                worker->gfx_data = uniform_data;
                break;
            }
            worker = worker->next;
        }

        scheduler_can_run = 1;
        scheduler_run(bsp_id);
        scheduler_can_run = 0;
    }else if (count <= cores) {
        worker = scheduler_workers;
        uint32_t offset = 0;
        while (worker != 0 && count > 0) {
            worker->gfx = true;
            worker->gfx_load = shader;
            worker->gfx_count = 1;
            worker->gfx_offset = offset;
            worker->gfx_data = uniform_data;
            offset += 1;
            worker = worker->next;
            count--;
        }

        scheduler_can_run = 1;
        worker = scheduler_workers;

        while (worker != 0) {
            if (worker->id != bsp_id && worker->gfx) {
                worker->running = true;
                smp_wake(bsp_id, worker->id);
            }
            worker = worker->next;
        }

        scheduler_run(bsp_id);
        scheduler_synchronize();
        scheduler_can_run = 0;
    }else{
        uint32_t stride = count / cores;
        uint32_t bsp_stride = count - (stride * (cores - 1));
        if (cores == 1) {
            bsp_stride = count;
        }
        worker = scheduler_workers;
        uint32_t offset = 0;
        while (worker != 0) {
            worker->gfx = true;
            worker->gfx_load = shader;
            if (worker->id == bsp_id) {
                worker->gfx_count = bsp_stride;
            }else{
                worker->gfx_count = stride;
            }
            worker->gfx_offset = offset;
            worker->gfx_data = uniform_data;
            offset += worker->gfx_count;
            worker = worker->next;
        }

        scheduler_can_run = 1;
        worker = scheduler_workers;

        while (worker != 0) {
            if (worker->id != bsp_id) {
                worker->running = true;
                smp_wake(bsp_id, worker->id);
            }
            worker = worker->next;
        }

        scheduler_run(bsp_id);
        scheduler_synchronize();
        scheduler_can_run = 0;
    }
}
