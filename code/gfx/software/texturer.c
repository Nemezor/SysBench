#include <render/texturer.h>
#include <stdint.h>
#include <math.h>

#define TEXTURER_MAX_UNITS 8

texture_t* texturer_textures[TEXTURER_MAX_UNITS];

uint32_t texturer_sample_texture(uint8_t index, float u, float v) {
    if (index >= TEXTURER_MAX_UNITS) {
        return 0;
    }
    texture_t* texture = texturer_textures[index];
    int iu = max(min((int)(u * texture->width), texture->width - 1), 0);
    int iv = max(min((int)(v * texture->height), texture->height - 1), 0);

    return texture->data[iv * texture->width + iu];
}

void texturer_bind_texture(uint8_t index, texture_t* texture) {
    if (index >= TEXTURER_MAX_UNITS) {
        return;
    }
    texturer_textures[index] = texture;
}
