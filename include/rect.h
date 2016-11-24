#pragma once

#include "lines.h"
#include <stdbool.h>

typedef struct {
	float x, y, width, height;
} Rect;
//Create a new recangle
Rect rect_new(float x, float y, float width, float height);
//Get the left side of the rect
Line rect_left(Rect r);
//Get the right side of the rect
Line rect_right(Rect r);
//Get the top of the rect
Line rect_top(Rect r);
//Get the bottom of the rect
Line rect_bottom(Rect r);
//Check if a rect contains a point
bool rect_contains(Rect r, Vector2 point);
//Check to see if a rect intersects a line
bool rect_intersects(Rect r, Line l);
