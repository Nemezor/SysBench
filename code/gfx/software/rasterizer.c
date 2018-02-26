#include <render/rasterizer.h>
#include <render/software.h>

inline int rasterizer_min(int a, int b) {
    return (a > b) ? b : a;
}

inline int rasterizer_max(int a, int b) {
    return (a > b) ? a : b;
}

void rasterizer_rasterize(uint32_t index, float* vertices, float* colors, float* uvs, float* normals, int viewport_left, int viewport_right, int viewport_top, int viewport_bottom, uint32_t fbwidth, uint32_t fbheight) {
    uint32_t i = index * 4 * 3;
    uint32_t color_index = index * 4 * 3;
    uint32_t uv_index = index * 2 * 3;
    uint32_t normal_index = index * 3 * 3;

    // perspective division
    float U1 = vertices[i    ] / vertices[i + 3];
    float V1 = vertices[i + 1] / vertices[i + 3];
    float Z1 = vertices[i + 2] / vertices[i + 3];
    float U2 = vertices[i + 4] / vertices[i + 7];
    float V2 = vertices[i + 5] / vertices[i + 7];
    float Z2 = vertices[i + 6] / vertices[i + 7];
    float U3 = vertices[i + 8] / vertices[i + 11];
    float V3 = vertices[i + 9] / vertices[i + 11];
    float Z3 = vertices[i + 10] / vertices[i + 11];

    // screenspace transform
    U1 = (U1 * 0.5f + 0.5f) * fbwidth;
    V1 = (V1 * 0.5f + 0.5f) * fbheight;
    U2 = (U2 * 0.5f + 0.5f) * fbwidth;
    V2 = (V2 * 0.5f + 0.5f) * fbheight;
    U3 = (U3 * 0.5f + 0.5f) * fbwidth;
    V3 = (V3 * 0.5f + 0.5f) * fbheight;
/*
    float x0 = vertices[i    ] - vertices[i + 8];
    float x1 = vertices[i + 4] - vertices[i + 8];
    float x2 = vertices[i + 8];
    float y0 = vertices[i + 1] - vertices[i + 9];
    float y1 = vertices[i + 5] - vertices[i + 9];
    float y2 = vertices[i + 9];
    float z0 = -vertices[i + 3] + vertices[i + 11];
    float z1 = -vertices[i + 7] + vertices[i + 11];
    float z2 = -vertices[i + 11];*/
    float x0 = U1 - U3;
    float x1 = U2 - U3;
    float x2 = U3;
    float y0 = V1 - V3;
    float y1 = V2 - V3;
    float y2 = V3;
    float z0 = -vertices[i + 3] + vertices[i + 11];
    float z1 = -vertices[i + 7] + vertices[i + 11];
    float z2 = -vertices[i + 11];
    float precomputedDenominator = (x0 * y1 - x1 * y0);

    int up = rasterizer_max(rasterizer_min(rasterizer_min((int)V1, (int)V2), (int)V3), viewport_top);
    int down = rasterizer_min(rasterizer_max(rasterizer_max((int)V1, (int)V2), (int)V3), viewport_bottom);
    int left = rasterizer_max(rasterizer_min(rasterizer_min((int)U1, (int)U2), (int)U3), viewport_left);
    int right = rasterizer_min(rasterizer_max(rasterizer_max((int)U1, (int)U2), (int)U3), viewport_right);

    for (int x = left; x < right; x++) {
        for (int y = up; y < down; y++) {
            float br1 = ((x - (int)U2) * ((int)V1 - (int)V2) - ((int)U1 - (int)U2) * (y - (int)V2));
            float br2 = ((x - (int)U3) * ((int)V2 - (int)V3) - ((int)U2 - (int)U3) * (y - (int)V3));
            float br3 = ((x - (int)U1) * ((int)V3 - (int)V1) - ((int)U3 - (int)U1) * (y - (int)V1));
            char b1 = br1 > 0;
            char b2 = br2 > 0;
            char b3 = br3 > 0;

            if ((b1 == b2) && (b2 == b3)) {
                float s = ((((float)x - x2) * y1 - x1 * ((float)y - y2)) / precomputedDenominator);
                float t = ((((float)y - y2) * x0 - y0 * ((float)x - x2)) / precomputedDenominator);

                float z = s * (Z1 - Z3) + t * (Z2 - Z3) + Z3;

                float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
                float u = 0.0f, v = 0.0f;
                float nx = 0.0f, ny = 0.0f, nz = 0.0f;

                if (uvs != 0) {
                    u = s * (uvs[uv_index    ] - uvs[uv_index + 4]) + t * (uvs[uv_index + 2] - uvs[uv_index + 4]) + uvs[uv_index + 4];
                    v = s * (uvs[uv_index + 1] - uvs[uv_index + 5]) + t * (uvs[uv_index + 3] - uvs[uv_index + 5]) + uvs[uv_index + 5];
                }
                if (colors != 0) {
                    r = s * (colors[color_index    ] - colors[color_index + 8]) + t * (colors[color_index + 4] - colors[color_index + 8]) + colors[color_index + 8];
                    g = s * (colors[color_index + 1] - colors[color_index + 9]) + t * (colors[color_index + 5] - colors[color_index + 9]) + colors[color_index + 9];
                    b = s * (colors[color_index + 2] - colors[color_index + 10]) + t * (colors[color_index + 6] - colors[color_index + 10]) + colors[color_index + 10];
                    a = s * (colors[color_index + 3] - colors[color_index + 11]) + t * (colors[color_index + 7] - colors[color_index + 11]) + colors[color_index + 11];
                }
                if (normals != 0) {
                    nx = s * (normals[normal_index    ] - normals[normal_index + 6]) + t * (normals[normal_index + 3] - normals[normal_index + 6]) + normals[normal_index + 6];
                    ny = s * (normals[normal_index + 1] - normals[normal_index + 7]) + t * (normals[normal_index + 4] - normals[normal_index + 7]) + normals[normal_index + 7];
                    nz = s * (normals[normal_index + 2] - normals[normal_index + 8]) + t * (normals[normal_index + 5] - normals[normal_index + 8]) + normals[normal_index + 8];
                }
                swrender_internal_fragment_shader(x, y, z, r, g, b, a, u, v, nx, ny, nz);
            }
        }
    }
}
