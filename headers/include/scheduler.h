#ifndef SYSTEM_SCHEDULER_H
#define SYSTEM_SCHEDULER_H

#include <stdint.h>
#include <benchmark.h>

struct scheduler_task {
    void (*main)(struct parameters* params);
    struct parameters* params;
};

void scheduler_add_worker(uint8_t id);
void scheduler_run(uint8_t id);
void scheduler_start(struct benchmark* bench, uint8_t bsp_id, char multicore);
void scheduler_secondly_callback();
void scheduler_dispatch_gfx(uint8_t bsp_id, uint32_t count, void (*shader)(uint32_t id, void* uniform_data), void* uniform_data);

#endif
