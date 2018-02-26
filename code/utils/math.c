#include <math.h>
#include <stdint.h>
#include <memory.h>

void matrix_identity(matrix_t* matrix) {
    memset(matrix, 0, sizeof(matrix_t));
    matrix->m00 = 1.0f;
    matrix->m11 = 1.0f;
    matrix->m22 = 1.0f;
    matrix->m33 = 1.0f;
}

void matrix_perspective(matrix_t* matrix, float field_of_view, float width, float height, float z_near, float z_far) {
    float fov = 1.0f / tan(field_of_view / 2.0f);
    float zp = z_far + z_near;
    float zm = z_far - z_near;

    matrix_identity(matrix);
    matrix->m00 = fov / (width / height);
    matrix->m11 = fov;
    matrix->m22 = -zp / zm;
    matrix->m23 = -1.0f;
    matrix->m32 = -(2.0f * z_far * z_near) / zm;
    matrix->m33 = 0.0f;
}

void matrix_translate(matrix_t* matrix, float x, float y, float z) {
    matrix->m30 = matrix->m00 * x + matrix->m10 * y + matrix->m20 * z + matrix->m30;
    matrix->m31 = matrix->m01 * x + matrix->m11 * y + matrix->m21 * z + matrix->m31;
    matrix->m32 = matrix->m02 * x + matrix->m12 * y + matrix->m22 * z + matrix->m32;
    matrix->m33 = matrix->m03 * x + matrix->m13 * y + matrix->m23 * z + matrix->m33;
}

void matrix_rotate(matrix_t* matrix, float angle, float x, float y, float z) {
    float s = sin(angle);
    float c = cos(angle);
    float C = 1.0f - c;
    float xx = x * x, xy = x * y, xz = x * z;
    float yy = y * y, yz = y * z;
    float zz = z * z;
    float rm00 = xx * C + c;
    float rm01 = xy * C + z * s;
    float rm02 = xz * C - y * s;
    float rm10 = xy * C - z * s;
    float rm11 = yy * C + c;
    float rm12 = yz * C + x * s;
    float rm20 = xz * C + y * s;
    float rm21 = yz * C - x * s;
    float rm22 = zz * C + c;
    float nm00 = matrix->m00 * rm00 + matrix->m10 * rm01 + matrix->m20 * rm02;
    float nm01 = matrix->m01 * rm00 + matrix->m11 * rm01 + matrix->m21 * rm02;
    float nm02 = matrix->m02 * rm00 + matrix->m12 * rm01 + matrix->m22 * rm02;
    float nm03 = matrix->m03 * rm00 + matrix->m13 * rm01 + matrix->m23 * rm02;
    float nm10 = matrix->m00 * rm10 + matrix->m10 * rm11 + matrix->m20 * rm12;
    float nm11 = matrix->m01 * rm10 + matrix->m11 * rm11 + matrix->m21 * rm12;
    float nm12 = matrix->m02 * rm10 + matrix->m12 * rm11 + matrix->m22 * rm12;
    float nm13 = matrix->m03 * rm10 + matrix->m13 * rm11 + matrix->m23 * rm12;
    float m20 = matrix->m00 * rm20 + matrix->m10 * rm21 + matrix->m20 * rm22;
    float m21 = matrix->m01 * rm20 + matrix->m11 * rm21 + matrix->m21 * rm22;
    float m22 = matrix->m02 * rm20 + matrix->m12 * rm21 + matrix->m22 * rm22;
    float m23 = matrix->m03 * rm20 + matrix->m13 * rm21 + matrix->m23 * rm22;
    matrix->m00 = nm00;
    matrix->m01 = nm01;
    matrix->m02 = nm02;
    matrix->m03 = nm03;
    matrix->m10 = nm10;
    matrix->m11 = nm11;
    matrix->m12 = nm12;
    matrix->m13 = nm13;
    matrix->m20 = m20;
    matrix->m21 = m21;
    matrix->m22 = m22;
    matrix->m23 = m23;
}

void matrix_rotate_translation(matrix_t* matrix, float angle, float x, float y, float z) {
    float s = sin(angle);
    float c = cos(angle);
    float C = 1.0f - c;
    float xx = x * x, xy = x * y, xz = x * z;
    float yy = y * y, yz = y * z;
    float zz = z * z;
    float rm00 = xx * C + c;
    float rm01 = xy * C + z * s;
    float rm02 = xz * C - y * s;
    float rm10 = xy * C - z * s;
    float rm11 = yy * C + c;
    float rm12 = yz * C + x * s;
    float rm20 = xz * C + y * s;
    float rm21 = yz * C - x * s;
    float rm22 = zz * C + c;
    float nm00 = rm00;
    float nm01 = rm01;
    float nm02 = rm02;
    float nm10 = rm10;
    float nm11 = rm11;
    float nm12 = rm12;
    matrix->m20 = rm20;
    matrix->m21 = rm21;
    matrix->m22 = rm22;
    matrix->m00 = nm00;
    matrix->m01 = nm01;
    matrix->m02 = nm02;
    matrix->m03 = 0.0f;
    matrix->m10 = nm10;
    matrix->m11 = nm11;
    matrix->m12 = nm12;
    matrix->m13 = 0.0f;
}

double cos(double x){
    x = fmod(x + M_PI, M_2PI) - M_PI;
	double sq_x = x*x, output = 1.0, qu_x = sq_x*sq_x;
	double fac[] = {2.0, 24.0, 720.0, 40320.0, 3628800.0, 479001600.0};//2!, 4!, 6!, 8!, 10!, 12!...
	for(int i = 0; i<6; i = i + 2){
		output -= (sq_x / fac[i]);
		sq_x = sq_x*qu_x;
	}
	sq_x = qu_x;
	for(int i = 1; i<6; i = i + 2){
		output += (sq_x / fac[i]);
		sq_x = sq_x*qu_x;
	}
	return output;
}

double sin(double x){
    x = -(fmod(x, M_2PI) - M_PI);
	double tr_x = x*x*x, output = x, qu_x = tr_x*x;
	double fac[] = {6.0, 120.0, 5040.0, 362880.0, 39916800.0, 6227020800.0};//3!, 5!, 7!, 9!, 11!, 13!...
	for(int i=0; i<6; i = i+2){
		output -= (tr_x/fac[i]);
		tr_x = tr_x*qu_x;
	}
	tr_x = qu_x*x;
	for(int i=1; i<6; i = i+2){
		output += (tr_x/fac[i]);
		tr_x = tr_x*qu_x;
	}
	return output;
}

double tan(double x) {
	return sin(x) / cos(x);
}

double fmod(double x, double div) {
	if (div != div || x != x) {
		return x;
	}
	if (div == 0) {
		return NAN;
	}
	return x - (int)(x / div) * div;
}

double radians(double angle) {
    return (angle / 360.0f) * M_2PI;
}

int min(int a, int b) {
    return (a > b) ? b : a;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}
