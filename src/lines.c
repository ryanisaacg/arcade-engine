#include "lines.h"
#include "math.h"

//Create a new vector with an x and a y 
Vector2 vec2_new(float x, float y) {
	return (Vector2){x, y};
}
//Find the square of the length of a vector
float vec2_len2(Vector2 vec) {
	return vec.x * vec.x + vec.y * vec.y;
}
//Find the length of a vector
float vec2_len(Vector2 vec) {
	return sqrt(vec2_len2(vec));
}
//Dot product two vectors
float vec2_dot(Vector2 a, Vector2 b) {
	return a.x * b.x + a.y * b.y;
}
//Cross product two vectors
float vec2_cross(Vector2 a, Vector2 b) {
	return a.x * b.y - a.y * b.x;
}
//Normalize a vector
Vector2 vec2_nor(Vector2 vec) {
	float len = vec2_len(vec);
	if (len != 0) {
		return vec2_new(vec.x / len, vec.y / len);
	} else {
		return vec;
	}
}
//Scale a vector
Vector2 vec2_scl(Vector2 vec, float scalar) {
	return vec2_new(vec.x * scalar, vec.y * scalar);
}
//Set the length of the vector
Vector2 vec2_with_len(Vector2 vec, float len) {
	return vec2_scl(vec2_nor(vec), len);
}
//Rotate a vector 
Vector2 vec2_rotate(Vector2 vec, float angle_in_degrees) {
	float radians = angle_in_degrees * M_PI / 180;
	float cosine = cos(radians);
	float sine = sin(radians);
	return vec2_new(vec.x * cosine - vec.y * sine, vec.x * sine + vec.y * cosine);
}
//Add two vectors
Vector2 vec2_add(Vector2 a, Vector2 b) {
	return vec2_new(a.x + b.x, a.y + b.y);
}
//Subtract two vectors
Vector2 vec2_sub(Vector2 a, Vector2 b) {
	return vec2_new(a.x - b.x, a.y - b.y);
}
//Invert a vector2
Vector2 vec2_neg(Vector2 vec) {
	return vec2_new(-vec.x, -vec.y);
}
//Create a new line between two points
Line line_new(Vector2 start, Vector2 end) {
	return (Line){start, end};
}
//Check if two lines intersect
//The end points are included
bool line_intersects(Line a, Line b) {
	Vector2 p = a.start;
	Vector2 q = b.start;
	Vector2 r = vec2_sub(a.end, a.start);
	Vector2 s = vec2_sub(b.end, b.start);
	//t = (q - p) x s / (r x s)
	//u = (q - p) x r / (r x s)
	float u_numerator = vec2_cross(vec2_sub(q, p), r);
	float t_numerator = vec2_cross(vec2_sub(q, p), s);
	float denominator = vec2_cross(r, s);
	if (denominator == 0.f) {
		if(u_numerator != 0.f)
			return false;
		float t0 = vec2_dot(vec2_sub(q, p), r) / vec2_dot(r, r);
		float t1 = t0 + vec2_dot(s, r) / vec2_dot(r, r);
		return t0 >= 0.f && t0 <= 1.f || t1 >= 0.f && t1 <= 1.f || copysign(1.f, t0) != copysign(1.f, t1);
	} else {
		float u = u_numerator / denominator;
		float t = t_numerator / denominator;
		return t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f;
	}
}
