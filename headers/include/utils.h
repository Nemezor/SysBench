#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <stdint.h>

extern void halt();
extern void stop();
extern void sti();
extern void cli();
extern void brk();
void iowait();
uint32_t int2strd(int64_t num, char* str);
uint32_t uint2strd(uint64_t num, char* str);
uint32_t int2strh(int64_t num, char* str);
uint32_t uint2strh(uint64_t num, char* str);

#endif
