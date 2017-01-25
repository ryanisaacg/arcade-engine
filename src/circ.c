#include "geom.h"

//Create a new circle
Circ circ_new(float x, float y, float radius) {
	return (Circ) {x, y, radius};
}
//Check if a point is in a circle
bool circ_contains(Circ c, Vector2 v) {
	Vector2 center = vec2_new(c.x, c.y);
	return vec2_len2(vec2_sub(center, v)) < (c.radius * c.radius);
}
//Check if a circle intersects a line
bool circ_intersects(Circ c, Line l) {
	Vector2 center = vec2_new(c.x, c.y);
	Vector2 line = vec2_sub(l.end, l.start);
	Vector2 dist = vec2_sub(center, l.start);
	Vector2 nor_line = vec2_nor(line);
	float product = vec2_dot(dist, nor_line);
	// Find the closest point on the line to check
	Vector2 check_point;
	if(product <= 0.f) {
		check_point = l.start;
	} else if(product >= 1.f) {
		check_point = l.end;
	} else {
		check_point = vec2_add(l.start, vec2_scl(nor_line, product));
	}
	// Check to see if the closest point is within the radius
	return vec2_len2(vec2_sub(center, check_point)) < (c.radius * c.radius);
}
//Get the rect bounding box of a circle
Rect circ_bounding_box(Circ circ) {
	return rect_new(circ.x - circ.radius, circ.y - circ.radius, 2 * circ.radius, 2 * circ.radius);
}
