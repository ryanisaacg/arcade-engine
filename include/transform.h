#pragma once

#include "matrix.h"

typedef struct {
	Matrix2 matrix;
	float rotation, scale;
	Vector2 position;
} Transform;

Transform trans_new(Vector2 position, float rotation, float scale);
Transform trans_new_identity();
Transform trans_new_rotation(float angle);
Transform trans_new_translation(Vector2 translate);
Transform trans_new_scale(float factor);
Transform trans_append(Transform dest, Transform source);
Transform trans_apply(Transform source, Vector2 target);

