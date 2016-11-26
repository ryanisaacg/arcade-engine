#pragma once

#include "circ.h"
#include "lines.h"
#include "poly.h"
#include "rect.h"

typedef struct {
	union {
		Rect r;
		Circ c;
		Polygon p;
	} data;
	enum { SHAPE_IS_RECT, SHAPE_IS_CIRC, SHAPE_IS_POLY} type;
} Shape;

Shape shape_rect(Rect r);
Shape shape_circ(Circ c);
Shape shape_poly(Polygon p);
void set_position(Shape *s, Vector2 position);
Vector2 get_position(Shape s);
bool shape_contains(Shape s, Vector2 point);
bool shape_intersects(Shape s, Line line);
Rect shape_bounding_box(Shape s);