#pragma once

#include "matrix.h"

struct Transform {
	Matrix3 matrix;
	float rotation;
	Vector2 scale, position;
};

Transform trans_new_identity();
Transform trans_new(Vector2 position, float rotation, Vector2 scale);
Transform trans_new_rotation(float angle);
Transform trans_new_translation(Vector2 translate);
Transform trans_new_scale(Vector2 factor);
Transform trans_append(Transform dest, Transform source);
Vector2 trans_apply(Transform source, Vector2 target);

