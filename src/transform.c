#include "transform.h"

#include <math.h>

Transform trans_new_identity() {
	return (Transform) { mat3_new_identity(), 0, vec2_new(1, 1), vec2_new(0, 0) };
}

Transform trans_new_rotation(float angle) {
	float c = cos(angle * M_PI / 180);
	float s = sin(angle * M_PI / 180);
	float array[3][3] = {
		{c, -s, 0},
		{s, c, 0},
		{0, 0, 1}};
	return (Transform) { mat3_new(array), angle, vec2_new(1, 1), vec2_new(0, 0) };
}

Transform trans_new_translation(Vector2 translate) {
	float array[3][3] = {
		{1, 0, translate.x},
		{0, 1, translate.y},
		{0, 0, 1}};
	return (Transform) { mat3_new(array), 0, vec2_new(1, 1), translate };
}

Transform trans_new_scale(Vector2 factor) {
	float array[3][3] = {
		{factor.x, 0, 0},
		{0, factor.y, 0},
		{0, 0, 1}};
	return (Transform) { mat3_new(array), 0, factor, vec2_new(0, 0) };
}

Transform trans_new(Vector2 position, float rotation, Vector2 scale) {
	Transform p, r, s;
	p = trans_new_translation(position);
	r = trans_new_rotation(rotation);
	s = trans_new_scale(scale);
	return trans_append(r, trans_append(s, p));
}

Transform trans_append(Transform dest, Transform source) {
	Matrix3 result = mat3_mult_mat3(dest.matrix, source.matrix);
	float rotation = dest.rotation + source.rotation;
	Vector2 scale = vec2_add(dest.scale, source.scale);
	Vector2 position = vec2_add(dest.position, source.position);
	return (Transform) { result, rotation, scale, position };
}

Vector2 trans_apply(Transform source, Vector2 target) {
	return mat3_mult_vec2(source.matrix, target);
}
