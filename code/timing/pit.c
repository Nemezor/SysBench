#include <pit.h>
#include <ports.h>
#include <stdint.h>
#include <utils.h>
#include <print.h>

#define PIT_IN_BIN 0
#define PIT_IN_BCD 1

#define PIT_MODE_INT_ON_TERM_COUNT 0 << 1
#define PIT_MODE_HW_RETRIG_ONE_SHOT 1 << 1
#define PIT_MODE_RATE_GEN 2 << 1
#define PIT_MODE_SQWAVE_GEN 3 << 1
#define PIT_MODE_SOFT_TRIG_STROBE 4 << 1
#define PIT_MODE_HW_TRIG_STROBE 5 << 1

#define PIT_ACCESS_LATCH 0 << 4
#define PIT_ACCESS_LOW_BYTE 1 << 4
#define PIT_ACCESS_HIGH_BYTE 2 << 4
#define PIT_ACCESS_BOTH_BYTE 3 << 4

#define PIT_CHANNEL_0 0 << 6
#define PIT_CHANNEL_1 1 << 6
#define PIT_CHANNEL_2 2 << 6
#define PIT_CHANNEL_RB 3 << 6

#define PIT_ONE_MILLIS_IN_TICKS 1193

uint64_t pit_sleep_counter = 0;
uint64_t pit_sleep_limit = 0;
uint16_t pit_current_value = 0;

void pit_load_value(uint16_t value) {
    pit_current_value = value;
    ports_out8(PORTS_PIT_CHANNEL_0, (char)(value));
    ports_out8(PORTS_PIT_CHANNEL_0, (char)(value >> 8));
}

void pit_irq0_trigger() {
    pit_sleep_counter++;
    ports_out8(PORTS_PIT_COMMAND, PIT_IN_BIN | PIT_MODE_INT_ON_TERM_COUNT | PIT_ACCESS_BOTH_BYTE | PIT_CHANNEL_0);
    pit_load_value(pit_current_value);
}

void pit_setup_sleep() {
    printf("[[PIT ]: Setting up PIT as sleep timer\n");
    ports_out8(PORTS_PIT_COMMAND, PIT_IN_BIN | PIT_MODE_INT_ON_TERM_COUNT | PIT_ACCESS_BOTH_BYTE | PIT_CHANNEL_0);
    pit_load_value(PIT_ONE_MILLIS_IN_TICKS);
}

void pit_sleep(uint64_t time) {
    pit_sleep_counter = 0;
    pit_sleep_limit = time;
    while (pit_sleep_counter <= pit_sleep_limit) {
        halt();
    }
}
