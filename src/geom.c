#include "geom.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"

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
//Check if two vectors are equal
bool vec2_eql(Vector2 a, Vector2 b) {
	return equal_float(a.x, b.x) && equal_float(a.y, b.y);
}
//Finds the angle a vector forms
float vec2_angle(Vector2 a) {
	return atan2(a.y, a.x) * 180 / M_PI;
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
		return (t0 >= 0.f && t0 <= 1.f) || (t1 >= 0.f && t1 <= 1.f) || 
			(copysign(1.f, t0) != copysign(1.f, t1));
	} else {
		float u = u_numerator / denominator;
		float t = t_numerator / denominator;
		return t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f;
	}
}
//Check if two lines are equal
bool line_eql(Line a, Line b) {
	return vec2_eql(a.start, b.start) && vec2_eql(a.end, b.end);
}

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
//Create a new polygon from a list of vertices
Polygon poly_new(Vector2 position, Vector2 *points, size_t num_points) {
	return (Polygon) {position, vec2_new(0, 0), points, num_points, 0, 1};
}
//Create a new polygon from a rectangle
Polygon poly_from_rect(Rect rect) {
	Vector2 *points = malloc(sizeof(*points) * 4);
	points[0] = vec2_new(0, 0);
	points[1] = vec2_new(rect.width, 0);
	points[2] = vec2_new(rect.width, rect.height);
	points[3] = vec2_new(0, rect.height);
	return poly_new(vec2_new(rect.x, rect.y), points, 4);
}
//Get a transformed vertex from a polygon
Vector2 poly_get_vertex(Polygon poly, size_t index) {
	Vector2 original = poly.points[index];
	Vector2 translated = vec2_sub(original, poly.origin);
	translated = vec2_rotate(translated, poly.rotation);
	translated = vec2_scl(translated, poly.scale);
	translated = vec2_add(translated, poly.origin);
	return vec2_add(translated, poly.pos);
}
//Find if a polygon contains a point
bool poly_contains(Polygon poly, Vector2 point) {
	size_t i = 0, j = poly.num_points - 1;
	bool c = 0;
	while(i < poly.num_points) {
		Vector2 vertexI = poly_get_vertex(poly, i);
		Vector2 vertexJ = poly_get_vertex(poly, j);
		if((vertexI.y > point.y) != (vertexJ.y > point.y) && 
				(point.x < (vertexJ.x - vertexI.x) * (point.y - vertexI.y) / (vertexJ.y - vertexI.y) + vertexI.x)) {
			c = !c; 
		}
		j = i;
		i += 1;
	}
	return c;
}
//Find if a polygon collided with a line
bool poly_intersects(Polygon poly, Line line) {
	for(size_t i = 0; i < poly.num_points; i++) {
		Vector2 one = poly_get_vertex(poly, i % poly.num_points);
		Vector2 two = poly_get_vertex(poly, (i + poly.num_points - 1) % poly.num_points);
		Line face = line_new(one, two);
		if(line_intersects(face, line)) {
			return true;
		}
	}
	return false;
}
//Find the bounds of a polygon as a rect
Rect poly_bounding_box(Polygon poly) {
	float x_min, y_min, x_max, y_max;
	Vector2 first = poly_get_vertex(poly, 0);
	x_min = x_max = first.x;
	y_min = y_max = first.y;
	for(size_t i = 1; i < poly.num_points; i++) {
		Vector2 point = poly_get_vertex(poly, i);
		if(point.x < x_min) {
			x_min = point.x;
		} else if(point.x > x_max) {
			x_max = point.y;
		}
		if(point.y < y_min) {
			y_min = point.y;
		} else if(point.y > y_max) {
			y_max = point.y;
		}
	}
	return rect_new(x_min, y_min, x_max - x_min, y_max - y_min);
}
void poly_destroy(Polygon poly) {
	free(poly.points);
}

//Create a new recangle
Rect rect_new(float x, float y, float width, float height) {
	return (Rect){x, y, width, height};
}
//Get the left side of the rect
Line rect_left(Rect r) {
	return line_new(vec2_new(r.x, r.y), vec2_new(r.x, r.y + r.height));
}
//Get the right side of the rect
Line rect_right(Rect r) {
	return line_new(vec2_new(r.x + r.width, r.y), vec2_new(r.x + r.width, r.y + r.height));
}
//Get the top of the rect
Line rect_top(Rect r) {
	return line_new(vec2_new(r.x, r.y), vec2_new(r.x + r.width, r.y));
}
//Get the bottom of the rect
Line rect_bottom(Rect r) {
	return line_new(vec2_new(r.x, r.y + r.height), vec2_new(r.x + r.width, r.y + r.height));
}
//Check if a rect contains a point
bool rect_contains(Rect r, Vector2 point) {
	return point.x >= r.x && point.y >= r.y && point.x < r.x + r.width && point.y < r.y + r.height;
}
//Check to see if a rect intersects a line
bool rect_intersects(Rect r, Line line) {
	return rect_contains(r, line.start) || rect_contains(r, line.end) || line_intersects(rect_left(r), line) ||
		line_intersects(rect_top(r), line) || line_intersects(rect_right(r), line) || line_intersects(rect_bottom(r), line);
}
Shape shape_rect(Rect r) {
	Shape s;
	s.data.r = r;
	s.type = SHAPE_IS_RECT;
	s.rot = 0;
	return s;
}
Shape shape_circ(Circ c) {
	Shape s;
	s.data.c = c;
	s.type = SHAPE_IS_CIRC;
	s.rot = 0;
	return s;
}
Shape shape_poly(Polygon p) {
	Shape s;
	s.data.p = p;
	s.type = SHAPE_IS_POLY;
	s.rot = p.rotation;
	return s;
}
void shape_set_position(Shape *s, Vector2 position) {
	switch(s->type) { 
		case SHAPE_IS_RECT:
			s->data.r.x = position.x;
			s->data.r.y = position.y;
			break;
		case SHAPE_IS_CIRC: 
			s->data.c.x = position.x + s->data.c.radius;
			s->data.c.y = position.y + s->data.c.radius;
			break;
		case SHAPE_IS_POLY:
			s->data.p.pos = position;
			break;
	}
}
Vector2 shape_get_position(Shape s) {
	switch(s.type) { 
		case SHAPE_IS_RECT:
			return vec2_new(s.data.r.x, s.data.r.y);
		case SHAPE_IS_CIRC: 
			return vec2_new(s.data.c.x - s.data.c.radius, s.data.c.y - s.data.c.radius);
		case SHAPE_IS_POLY:
			return s.data.p.pos;
	}
	fprintf(stderr, "shape_get_position called with an invalid type");
	exit(-1);
}
bool shape_contains(Shape s, Vector2 point) {
	switch(s.type) { 
		case SHAPE_IS_RECT:
			return rect_contains(s.data.r, point);
		case SHAPE_IS_CIRC: 
			return circ_contains(s.data.c, point);
		case SHAPE_IS_POLY:
			return poly_contains(s.data.p, point);
	}
	return false;
}
bool shape_intersects(Shape s, Line line) {
	switch(s.type) { 
		case SHAPE_IS_RECT:
			return rect_intersects(s.data.r, line);
		case SHAPE_IS_CIRC: 
			return circ_intersects(s.data.c, line);
		case SHAPE_IS_POLY:
			return poly_intersects(s.data.p, line);
	}
	return false;
}
Rect shape_bounding_box(Shape s) {
	switch(s.type) {
		case SHAPE_IS_RECT:
			return s.data.r;
		case SHAPE_IS_CIRC:
			return circ_bounding_box(s.data.c);
		case SHAPE_IS_POLY:
			return poly_bounding_box(s.data.p);
	}
	return rect_new(0, 0, 0, 0);
}
float shape_get_rotation(Shape s) {
	switch(s.type) {
		case SHAPE_IS_RECT:
			return 0;
		case SHAPE_IS_CIRC:
			return s.rot;
		case SHAPE_IS_POLY:
			return s.rot;
	}
	return 0;
}
void shape_set_rotation(Shape *s, float rotation) {
	switch(s->type) {
		case SHAPE_IS_RECT:
			s->data.p = poly_from_rect(s->data.r);
			s->type = SHAPE_IS_POLY;
			shape_set_rotation(s, rotation);
			break;
		case SHAPE_IS_CIRC:
			s->rot = rotation;
			break;
		case SHAPE_IS_POLY:
			s->data.p.rotation = rotation;
			s->rot = rotation;
			break;
	}
}
void shape_rotate(Shape *s, float amount) {
	float current = shape_get_rotation(*s);
	shape_set_rotation(s, current + amount);
}
Vector2 shape_get_center(Shape s) {
	Rect bounds = shape_bounding_box(s);
	return vec2_new(bounds.x + bounds.width / 2, bounds.y + bounds.height / 2);
}
bool engulfs_circ(Circ outer, Circ inner);
bool engulfs_rect(Rect outer, Rect inner) {
	return inner.x >= outer.x && inner.x + inner.width <= outer.x + outer.width && 
		inner.y >= outer.y && inner.y + inner.height <= outer.y + outer.height;
}
bool engulfs_rect_circ(Rect outer, Circ inner) {
	return engulfs_rect(outer, circ_bounding_box(inner));
}
bool engulfs_circ_rect(Circ outer, Rect inner) {
	return engulfs_rect(circ_bounding_box(outer), inner);
}
bool overlaps_circ(Circ a, Circ b) {
	Vector2 center = vec2_new(a.x, a.y);
	Vector2 other = vec2_new(b.x, b.y);
	float radius_sum = a.radius + b.radius;
	return vec2_len2(vec2_sub(center, other)) < (radius_sum * radius_sum);
}
bool overlaps_rect(Rect a, Rect b) {
	return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}
bool overlaps_rect_circ(Rect r, Circ c) {
	Vector2 closest;
	if(c.x < r.x) {
		closest.x = r.x;
	} else if(c.x > r.x + r.width) {
		closest.x = r.x + r.width;
	} else {
		closest.x = c.x;
	}
	if(c.y < r.y) {
		closest.y = r.y;
	} else if(c.y > r.y + r.height) {
		closest.y = r.y + r.height;
	} else {
		closest.y = c.y;
	}
	closest = vec2_sub(closest, vec2_new(c.x, c.y));
	return (closest.x * closest.x) + (closest.y * closest.y) < c.radius * c.radius;
}
#define POLY_OVERLAP(poly, other, check_line) \
	for(size_t i = 0; i < poly.num_points; i++) { \
		Vector2 one = poly_get_vertex(poly, i % poly.num_points); \
		Vector2 two = poly_get_vertex(poly, (i + poly.num_points - 1) % poly.num_points); \
		Line face = line_new(one, two); \
		if(check_line(other, face)) { \
			return true; \
		} \
	} \
	return false;
bool overlaps_poly(Polygon a, Polygon b) {
	POLY_OVERLAP(a, b, poly_intersects);
}
bool overlaps_poly_rect(Polygon a, Rect b) {
	POLY_OVERLAP(a, b, rect_intersects);
}
bool overlaps_poly_circ(Polygon a, Circ b) {
	POLY_OVERLAP(a, b, circ_intersects);
}
bool overlaps_shape(Shape a, Shape b) {
	if(a.type == b.type) {
		switch(a.type) {
			case SHAPE_IS_CIRC:
				return overlaps_circ(a.data.c, b.data.c);
			case SHAPE_IS_POLY:
				return overlaps_poly(a.data.p, b.data.p);
			case SHAPE_IS_RECT:
				return overlaps_rect(a.data.r, b.data.r);
		}
	} else {
		switch(a.type) {
			case SHAPE_IS_CIRC:
				return overlaps_shape(b, a);
			case SHAPE_IS_RECT:
				switch(b.type) {
					case SHAPE_IS_CIRC:
						return overlaps_rect_circ(a.data.r, b.data.c);
					case SHAPE_IS_RECT:
						return overlaps_rect(a.data.r, b.data.r);
					case SHAPE_IS_POLY:
						return overlaps_poly_rect(b.data.p, a.data.r);
				}
			case SHAPE_IS_POLY:
				switch(b.type) {
					case SHAPE_IS_CIRC:
						return overlaps_poly_circ(a.data.p, b.data.c);
					case SHAPE_IS_RECT:
						return overlaps_poly_rect(a.data.p, b.data.r);
					case SHAPE_IS_POLY:
						return overlaps_poly(a.data.p, b.data.p);
				}
		}
	}
	return false;
}
