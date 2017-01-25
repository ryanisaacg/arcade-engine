#pragma once

#include "forward-decl.h"

struct Shape {
	union {
		Rect r;
		Circ c;
		Polygon p;
	} data;
	enum { SHAPE_IS_RECT, SHAPE_IS_CIRC, SHAPE_IS_POLY} type;
	float rot;
};

#include "circ.h"
#include "lines.h"
#include "poly.h"
#include "rect.h"

Shape shape_rect(Rect r);
Shape shape_circ(Circ c);
Shape shape_poly(Polygon p);
void shape_set_position(Shape *s, Vector2 position);
Vector2 shape_get_position(Shape s);
bool shape_contains(Shape s, Vector2 point);
bool shape_intersects(Shape s, Line line);
Rect shape_bounding_box(Shape s);
float shape_get_rotation(Shape s);
void shape_set_rotation(Shape *s, float rotation);
void shape_rotate(Shape *s, float amount);
Vector2 shape_get_center(Shape s);
