#ifndef GFX_SOFTWARE_TEXTURER_H
#define GFX_SOFTWARE_TEXTURER_H

#include <stdint.h>
#include <render/textures.h>

uint32_t texturer_sample_texture(uint8_t index, float u, float v);
void texturer_bind_texture(uint8_t index, texture_t* texture);

#endif
