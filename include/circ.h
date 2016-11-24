#pragma once

#include "lines.h"
#include "rect.h"
#include <stdbool.h>

typedef struct {
	float x, y, radius;
} Circ;

//Create a new circle
Circ circ_new(float x, float y, float radius);
//Check if a point is in a circle
bool circ_contains(Circ c, Vector2 v);
//Check if a circle intersects a line
bool circ_intersects(Circ c, Line l);
//Get the rect bounding box of a circle
Rect circ_bounding_box(Circ circ);

