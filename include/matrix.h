#pragma once

#include "lines.h"

/*
 * a b
 * c d
 */

typedef struct {
	float a, b, c, d;
} Matrix2;

Matrix2 mat2_new(float a, float b, float c, float d);
Matrix2 mat2_mult_mat2(Matrix2 m, Matrix2 n);
Vector2 mat2_mult_vec2(Matrix2 m, Vector2 n);
