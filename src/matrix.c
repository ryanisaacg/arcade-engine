#include "matrix.h"

Matrix2 mat2_new(float a, float b, float c, float d) {
	return (Matrix2) { a, b, c, d };
}

Matrix2 mat2_mult_mat2(Matrix2 m, Matrix2 n) {
	Vector2 top 	= vec2_new(m.a, m.b);
	Vector2 bot 	= vec2_new(m.c, m.d);
	Vector2 left 	= vec2_new(n.a, n.c);
	Vector2 right 	= vec2_new(n.b, n.d);
	return mat2_new(vec2_dot(top, left), vec2_dot(top, right), 
					vec2_dot(bot, left), vec2_dot(bot, right));
}


Vector2 mat2_mult_vec2(Matrix2 m, Vector2 n) {
	Vector2 top = vec2_new(m.a, m.b);
	Vector2 bot = vec2_new(m.c, m.d);
	return vec2_new(vec2_dot(top, n), vec2_dot(bot, n));
}
