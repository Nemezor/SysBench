#ifndef UTILS_FPU_H
#define UTILS_FPU_H

#include <stdint.h>

void fpu_init();
extern void fpu_init_x87();
extern void fpu_init_sse();
extern void fpu_save(void* data);
extern void fpu_load(void* data);

#endif
