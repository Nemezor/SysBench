#include <fpu.h>
#include <print.h>

void fpu_init() {
    printf("[[FPU ]: Enabling x87 FPU\n");
    fpu_init_x87();
    printf("[[FPU ]: Enabling SSE\n");
    fpu_init_sse();
}
