#ifndef GFX_SOFTWARE_TEXTURES_H
#define GFX_SOFTWARE_TEXTURES_H

#include <stdint.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t data[];
} texture_t;

#endif
