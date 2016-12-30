#include "transform.h"

#include <math.h>

Transform trans_new_identity() {
	return (Transform) { mat2_new(1, 0, 1, 0), 0, 1, vec2_new(0, 0) };
}

Transform trans_new_rotation(float angle) {
	float c = cos(angle * M_PI / 180);
	float s = sin(angle * M_PI / 180);
	return (Transform) { mat2_new(c, -s, s, c), angle, 1, vec2_new(0, 0) };
}

Transform trans_new_translation(Vector2 translate);
Transform trans_new_scale(float factor);
Transform trans_new(Vector2 position, float rotation, float scale);
Transform trans_append(Transform dest, Transform source);
Transform trans_apply(Transform source, Vector2 target);
