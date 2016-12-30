#include "shape.h"

#include <stdio.h>
#include <stdlib.h>

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
