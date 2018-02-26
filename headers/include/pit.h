#ifndef TIMING_PIT_H
#define TIMING_PIT_H

#include <stdint.h>

void pit_sleep(uint64_t millis);
void pit_setup_sleep();
void pit_irq0_trigger();

#endif
