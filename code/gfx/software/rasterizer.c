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
    float X1 = vertices[i    ] / vertices[i + 3];
    float Y1 = vertices[i + 1] / vertices[i + 3];
    float Z1 = vertices[i + 3];
    float rZ1 = 1.0f / Z1;
    float X2 = vertices[i + 4] / vertices[i + 7];
    float Y2 = vertices[i + 5] / vertices[i + 7];
    float Z2 = vertices[i + 7];
    float rZ2 = 1.0f / Z2;
    float X3 = vertices[i + 8] / vertices[i + 11];
    float Y3 = vertices[i + 9] / vertices[i + 11];
    float Z3 = vertices[i + 11];
    float rZ3 = 1.0f / Z3;

    // screenspace transform
    X1 = (-X1 * 0.5f + 0.5f) * fbwidth;
    Y1 = (Y1 * 0.5f + 0.5f) * fbheight;
    X2 = (-X2 * 0.5f + 0.5f) * fbwidth;
    Y2 = (Y2 * 0.5f + 0.5f) * fbheight;
    X3 = (-X3 * 0.5f + 0.5f) * fbwidth;
    Y3 = (Y3 * 0.5f + 0.5f) * fbheight;

    float U1 = uvs[uv_index    ] * rZ1;
    float V1 = uvs[uv_index + 1] * rZ1;
    float U2 = uvs[uv_index + 2] * rZ2;
    float V2 = uvs[uv_index + 3] * rZ2;
    float U3 = uvs[uv_index + 4] * rZ3;
    float V3 = uvs[uv_index + 5] * rZ3;
    float R1 = colors[color_index    ] * rZ1;
    float G1 = colors[color_index + 1] * rZ1;
    float B1 = colors[color_index + 2] * rZ1;
    float A1 = colors[color_index + 3] * rZ1;
    float R2 = colors[color_index + 4] * rZ2;
    float G2 = colors[color_index + 5] * rZ2;
    float B2 = colors[color_index + 6] * rZ2;
    float A2 = colors[color_index + 7] * rZ2;
    float R3 = colors[color_index + 8] * rZ3;
    float G3 = colors[color_index + 9] * rZ3;
    float B3 = colors[color_index + 10] * rZ3;
    float A3 = colors[color_index + 11] * rZ3;
    float NX1 = normals[normal_index    ] * rZ1;
    float NY1 = normals[normal_index + 1] * rZ1;
    float NZ1 = normals[normal_index + 2] * rZ1;
    float NX2 = normals[normal_index + 3] * rZ2;
    float NY2 = normals[normal_index + 4] * rZ2;
    float NZ2 = normals[normal_index + 5] * rZ2;
    float NX3 = normals[normal_index + 6] * rZ3;
    float NY3 = normals[normal_index + 7] * rZ3;
    float NZ3 = normals[normal_index + 8] * rZ3;

    float Y2mY3 = Y2 - Y3;
    float X3mX2 = X3 - X2;
    float X1mX3 = X1 - X3;
    float Y3mY1 = Y3 - Y1;
    float denom = (Y2mY3 * X1mX3 + X3mX2 * (Y1 - Y3));

    int up = rasterizer_max(rasterizer_min(rasterizer_min((int)Y1, (int)Y2), (int)Y3), viewport_top);
    int down = rasterizer_min(rasterizer_max(rasterizer_max((int)Y1, (int)Y2), (int)Y3), viewport_bottom);
    int left = rasterizer_max(rasterizer_min(rasterizer_min((int)X1, (int)X2), (int)X3), viewport_left);
    int right = rasterizer_min(rasterizer_max(rasterizer_max((int)X1, (int)X2), (int)X3), viewport_right);

    for (int x = left; x <= right; x++) {
        for (int y = up; y <= down; y++) {
            float xmX3 = x - X3;
            float ymY3 = y - Y3;

            float w1 = (Y2mY3 * xmX3 + X3mX2 * ymY3) / denom;
            float w2 = (Y3mY1 * xmX3 + X1mX3 * ymY3) / denom;
            float w3 = 1.0f - w1 - w2;

            if (w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f) {
                float z = w1 * Z1 + w2 * Z2 + w3 * Z3;
                float rz = 1.0f / (w1 * rZ1 + w2 * rZ2 + w3 * rZ3);

                float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
                float u = 0.0f, v = 0.0f;
                float nx = 0.0f, ny = 0.0f, nz = 0.0f;

                if (uvs != 0) {
                    u = (w1 * U1 + w2 * U2 + w3 * U3) * rz;
                    v = (w1 * V1 + w2 * V2 + w3 * V3) * rz;
                }
                if (colors != 0) {
                    r = (w1 * R1 + w2 * R2 + w3 * R3) * rz;
                    g = (w1 * G1 + w2 * G2 + w3 * G3) * rz;
                    b = (w1 * B1 + w2 * B2 + w3 * B3) * rz;
                    a = (w1 * A1 + w2 * A2 + w3 * A3) * rz;
                }
                if (normals != 0) {
                    nx = (w1 * NX1 + w2 * NX2 + w3 * NX3) * rz;
                    ny = (w1 * NY1 + w2 * NY2 + w3 * NY3) * rz;
                    nz = (w1 * NZ1 + w2 * NZ2 + w3 * NZ3) * rz;
                }
                swrender_internal_fragment_shader(x, y, z, r, g, b, a, u, v, nx, ny, nz);
            }
        }
    }
}
