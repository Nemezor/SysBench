#ifndef SYSTEM_SYSTEM_H
#define SYSTEM_SYSTEM_H

#include <stdint.h>

extern void system_call(char number, uint32_t code);
void sleep(uint32_t millis);
uint64_t current_time();
void system_irq_increment_time();

#endif
