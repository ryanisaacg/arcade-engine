#pragma once

#include "lines.h"

typedef struct {
	float x, y, z;
} Vector3;

Vector3 vec3_new(float x, float y, float z);
float vec3_dot(Vector3 a, Vector3 b);

struct Matrix3{
	float data[3][3];
};

Matrix3 mat3_new_identity();
Matrix3 mat3_new(float data[3][3]);
Matrix3 mat3_mult_mat3(Matrix3 m, Matrix3 n);
Vector3 mat3_mult_vec3(Matrix3 m, Vector3 n);
Vector2 mat3_mult_vec2(Matrix3 m, Vector2 n);
