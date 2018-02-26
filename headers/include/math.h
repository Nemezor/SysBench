#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#include <stdint.h>

#define M_E              2.718281828    /* e */
#define M_LOG2E	       1.442695040    /* log_2 e */
#define M_LOG10E         0.434294481    /* log_10 e */
#define M_LN2            0.693147180    /* log_e 2 */
#define M_LN10           2.302585092    /* log_e 10 */
#define M_2PI            6.283185307    /* 2*pi */
#define M_PI             3.141592653	/* pi */
#define M_PI_2           1.570796326	/* pi/2 */
#define M_PI_4           0.785398163	/* pi/4 */
#define M_1_PI           0.318309886	/* 1/pi */
#define M_2_PI           0.636619772	/* 2/pi */
#define M_2_SQRTPI       1.128379167	/* 2/sqrt(pi) */
#define M_SQRT2          1.414213562	/* sqrt(2) */
#define M_SQRT1_2        0.707106781	/* 1/sqrt(2) */
#define NAN 0x7FC00000

typedef struct {
    float m00, m10, m20, m30;
    float m01, m11, m21, m31;
    float m02, m12, m22, m32;
    float m03, m13, m23, m33;
} matrix_t;

void matrix_identity(matrix_t* matrix);
void matrix_perspective(matrix_t* matrix, float field_of_view, float width, float height, float z_near, float z_far);
void matrix_translate(matrix_t* matrix, float x, float y, float z);
void matrix_rotate(matrix_t* matrix, float angle, float x, float y, float z);
void matrix_rotate_translation(matrix_t* matrix, float angle, float x, float y, float z);
double sin(double angle);
double cos(double angle);
double tan(double angle);
double fmod(double x, double div);
double radians(double angle);
int min(int a, int b);
int max(int a, int b);

#endif
