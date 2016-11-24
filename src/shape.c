#include "shape.h"

Shape shape_rect(Rect r) {
	Shape s;
	s.data.r = r;
	s.type = SHAPE_IS_RECT;
	return s;
}
Shape shape_circ(Circ c) {
	Shape s;
	s.data.c = c;
	s.type = SHAPE_IS_CIRC;
	return s;
}
Shape shape_poly(Polygon p) {
	Shape s;
	s.data.p = p;
	s.type = SHAPE_IS_POLY;
	return s;
}
void set_position(Shape *s, Vector2 position) {
	switch(s->type) { 
		case SHAPE_IS_RECT:
			s->data.r.x = position.x;
			s->data.r.y = position.y;
		case SHAPE_IS_CIRC: 
			s->data.c.x = position.x;
			s->data.c.y = position.y;
		case SHAPE_IS_POLY:
			s->data.p.pos = position;
	}
}
Vector2 get_position(Shape s) {
	switch(s.type) { 
		case SHAPE_IS_RECT:
			return vec2_new(s.data.r.x, s.data.r.y);
		case SHAPE_IS_CIRC: 
			return vec2_new(s.data.c.x, s.data.c.y);
		case SHAPE_IS_POLY:
			return s.data.p.pos;
	}
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
