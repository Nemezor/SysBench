#ifndef GFX_SOFTWARE_RASTERIZER_H
#define GFX_SOFTWARE_RASTERIZER_H

#include <stdint.h>

void rasterizer_rasterize(uint32_t i, float* vertices, float* colors, float* uvs, float* normals, int viewport_left, int viewport_right, int viewport_top, int viewport_bottom, uint32_t fbwidth, uint32_t fbheight);

#endif
