#include "matrix.h"
#include <string.h>

Vector3 vec3_new(float x, float y, float z) {
	return (Vector3) { x, y, z };
}

float vec3_dot(Vector3 a, Vector3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Matrix3 mat3_new_identity() {
	float in[3][3] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1}};
	return mat3_new(in);
}

Matrix3 mat3_new(float in[3][3]) {
	Matrix3 mat;
	memcpy(mat.data, in, 9 * sizeof(float));
	return mat;
}

Matrix3 mat3_mult_mat3(Matrix3 m, Matrix3 n) {
	Matrix3 result;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			Vector3 a = vec3_new(m.data[i][0], m.data[i][1], m.data[i][2]);
			Vector3 b = vec3_new(n.data[0][j], n.data[1][j], n.data[2][j]);
			result.data[i][j] = vec3_dot(a, b);
		}
	}
	return result;
}

Vector3 mat3_mult_vec3(Matrix3 m, Vector3 n) {
	float result[3];
	for(int i = 0; i < 3; i++) {
		result[i] = vec3_dot(vec3_new(m.data[i][0], m.data[i][1], m.data[i][2]), n);
	}
	return vec3_new(result[0], result[1], result[2]);
}

Vector2 mat3_mult_vec2(Matrix3 m, Vector2 n) {
	Vector3 result = mat3_mult_vec3(m, vec3_new(n.x, n.y, 1));
	return vec2_new(result.x, result.y);
}
