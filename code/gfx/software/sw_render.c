#include <stdint.h>
#include <memory.h>
#include <gfx_device.h>
#include <render/software.h>
#include <print.h>
#include <scheduler.h>
#include <render/rasterizer.h>
#include <math.h>
#include <render/texturer.h>
#include <render/textures.h>

uint32_t* swrender_framebuffer_front;
uint32_t* swrender_framebuffer_back;
float* swrender_framebuffer_depth;
uint32_t swrender_framebuffer_width;
uint32_t swrender_framebuffer_height;
uint8_t swrender_bsp_id;
struct gfx_render swrender_renderer;
uint32_t swrender_viewport_left, swrender_viewport_right, swrender_viewport_top, swrender_viewport_bottom;

matrix_t swrender_transformation;
matrix_t swrender_projection;

struct swrender_cnd {
    uint32_t color;
    float depth;
};

struct swrender_shader_data {
    float* vertices;
    float* uvs;
    float* colors;
    float* normals;
};

void swrender_setpixel(uint32_t x, uint32_t y, uint32_t c) {
    swrender_framebuffer_back[y * swrender_framebuffer_width + x] = c;
}

uint32_t swrender_getwidth() {
    return swrender_framebuffer_width;
}

uint32_t swrender_getheight() {
    return swrender_framebuffer_height;
}

void swrender_set_transformation(matrix_t* matrix) {
    memcpy(matrix, &swrender_transformation, sizeof(matrix_t));
}

void swrender_set_projection(matrix_t* matrix) {
    memcpy(matrix, &swrender_projection, sizeof(matrix_t));
}

void swrender_internal_shader_clear(uint32_t id, void* uniform_data) {
    volatile struct swrender_cnd* color_and_depth = (struct swrender_cnd*)uniform_data;
    uint32_t start = id * (swrender_framebuffer_width * swrender_framebuffer_height);
    uint32_t end = start + (swrender_framebuffer_width * swrender_framebuffer_height);
    for (uint32_t i = start; i < end; i++) {
        swrender_framebuffer_back[i] = color_and_depth->color;
        swrender_framebuffer_depth[i] = color_and_depth->depth;
    }
}

void swrender_clear(uint32_t color, float depth) {
    struct swrender_cnd color_and_depth;
    color_and_depth.color = color;
    color_and_depth.depth = depth;
    swrender_run(1, &swrender_internal_shader_clear, &color_and_depth);
}

void swrender_mulmat(matrix_t* matrix, float* data, uint32_t index) {
    float a = matrix->m00 * data[index] + matrix->m10 * data[index + 1] + matrix->m20 * data[index + 2] + matrix->m30 * data[index + 3];
    float b = matrix->m01 * data[index] + matrix->m11 * data[index + 1] + matrix->m21 * data[index + 2] + matrix->m31 * data[index + 3];
    float c = matrix->m02 * data[index] + matrix->m12 * data[index + 1] + matrix->m22 * data[index + 2] + matrix->m32 * data[index + 3];
    float d = matrix->m03 * data[index] + matrix->m13 * data[index + 1] + matrix->m23 * data[index + 2] + matrix->m33 * data[index + 3];
    data[index    ] = a;
    data[index + 1] = b;
    data[index + 2] = c;
    data[index + 3] = d;
}

void swrender_internal_vertex_shader(uint32_t id, void* uniform_data) {
    struct swrender_shader_data* data = (struct swrender_shader_data*)uniform_data;
    float* vertices = data->vertices;
    uint32_t i = id * 4;

    // transform and project
    vertices[i + 3] = 1.0f;
    swrender_mulmat(&swrender_transformation, vertices, i);
    swrender_mulmat(&swrender_projection, vertices, i);
}

inline void swrender_internal_fragment_shader(float x, float y, float z, float r, float g, float b, float a, float u, float v, float nx, float ny, float nz) {
    uint32_t index = y * swrender_framebuffer_width + x;

    if (swrender_framebuffer_depth[index] <= z) {
        uint32_t texture_color = swrender_renderer.texture_sample(0, u, v);

        r = ((float)((texture_color >> 16) & 0xFF) / 255.0f);
        g = ((float)((texture_color >> 8) & 0xFF) / 255.0f);
        b = ((float)(texture_color & 0xFF) / 255.0f);
        a = ((float)((texture_color >> 24) & 0xFF) / 255.0f);

        uint32_t color = 0;
        //a = 0.5f;
        if (a != 1.0f) {
            uint32_t dst_color = swrender_framebuffer_back[index];
            float one_minus_a = 1.0f - a;

            r = (r * a) + (((float)((dst_color >> 16) & 0xFF) / 255.0f) * one_minus_a);
            g = (g * a) + (((float)((dst_color >> 8) & 0xFF) / 255.0f) * one_minus_a);
            b = (b * a) + (((float)(dst_color & 0xFF) / 255.0f) * one_minus_a);

            color = (((uint32_t)(r * 255) & 0xFF) << 16) | (((uint32_t)(g * 255) & 0xFF) << 8) | ((uint32_t)(b * 255) & 0xFF);
        }else{
            color = (((uint32_t)(r * 255) & 0xFF) << 16) | (((uint32_t)(g * 255) & 0xFF) << 8) | ((uint32_t)(b * 255) & 0xFF);
        }

        swrender_framebuffer_depth[index] = z;
        swrender_framebuffer_back[index] = color;
    }
}

void swrender_internal_triangle_shader(uint32_t id, void* uniform_data) {
    volatile struct swrender_shader_data* data = (struct swrender_shader_data*)uniform_data;

    //TODO: change viewport l/r/t/b to an actual viewport for scissor masks
    rasterizer_rasterize(id, data->vertices, data->colors, data->uvs, data->normals, swrender_viewport_left, swrender_viewport_right, swrender_viewport_top, swrender_viewport_bottom, swrender_framebuffer_width, swrender_framebuffer_height);
}

void swrender_draw_triangles(uint32_t triangle_count, float* vertices, float* uvs, float* colors, float* normals) {
    uint32_t vertex_count = triangle_count * 3;

    struct swrender_shader_data data;
    data.vertices = vertices;
    data.colors = colors;
    data.uvs = uvs;
    data.normals = normals;

    swrender_run(vertex_count, &swrender_internal_vertex_shader, &data);
    swrender_run(triangle_count, &swrender_internal_triangle_shader, &data);
}

void swrender_set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    swrender_viewport_left = max(x, 0);
    swrender_viewport_top = max(y, 0);
    swrender_viewport_right = min(x + width, swrender_framebuffer_width);
    swrender_viewport_bottom = min(y + height, swrender_framebuffer_height);
}

void swrender_initialize(uint8_t bsp_id, uint32_t width, uint32_t height) {
    swrender_framebuffer_front = memory_reserve(width * height * sizeof(uint32_t));
    swrender_framebuffer_back = memory_reserve(width * height * sizeof(uint32_t));
    swrender_framebuffer_depth = memory_reserve(width * height * sizeof(float));
    swrender_framebuffer_width = width;
    swrender_framebuffer_height = height;
    swrender_bsp_id = bsp_id;

    matrix_identity(&swrender_transformation);
    matrix_identity(&swrender_projection);

    swrender_renderer.get_width = &swrender_getwidth;
    swrender_renderer.get_height = &swrender_getheight;
    swrender_renderer.draw_pixel = &swrender_setpixel;
    swrender_renderer.draw_triangles = &swrender_draw_triangles;
    swrender_renderer.set_projection = &swrender_set_projection;
    swrender_renderer.set_transformation = &swrender_set_transformation;
    swrender_renderer.clear = &swrender_clear;
    swrender_renderer.set_viewport = &swrender_set_viewport;
    swrender_renderer.texture_sample = &texturer_sample_texture;
    swrender_renderer.texture_bind = &texturer_bind_texture;

    printf("[[GFX ]: Software Renderer initialized\n");
    printf("[[GFX ]: FrameBuffer size: %ux%u\n", swrender_framebuffer_width, swrender_framebuffer_height);
}

void swrender_update_screen(uint32_t* destination) {
    swrender_framebuffercpy(swrender_framebuffer_back, swrender_framebuffer_front, destination, (swrender_framebuffer_width * swrender_framebuffer_height) / 2);
}

struct gfx_render* swrender_get_gfx_render() {
    return &swrender_renderer;
}

void swrender_run(uint32_t count, void (shader)(uint32_t id, void* uniform_data), void* uniform_data) {
    scheduler_dispatch_gfx(swrender_bsp_id, count, shader, uniform_data);
}
