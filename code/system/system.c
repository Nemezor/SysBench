#include <system.h>
#include <stdint.h>
#include <pit.h>
#include <scheduler.h>

uint64_t system_current_time_millis = 0;

void sleep(uint32_t millis) {
    pit_sleep(millis);
}

uint64_t current_time() {
    return system_current_time_millis;
}

void system_irq_increment_time() {
    system_current_time_millis++;
    if (system_current_time_millis % 1000 == 0) {
        scheduler_secondly_callback();
    }
}
