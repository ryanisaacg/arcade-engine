#pragma once

#include <stdbool.h>

#include "lines.h"

typedef struct {
	float x, y, radius;
} Circ;

//Create a new circle
Circ circ_new(float x, float y, float radius);
//Check if a point is in a circle
bool circ_contains(Circ c, Vector2 v);
//Check if a circle intersects a line
bool circ_intersects(Circ c, Line l);
//Check if a circ overlaps another circ
bool circ_overlaps_circ(Circ a, Circ b);

