#pragma once

#include "circ.h"
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
//Check if the inner rect is entirely contained within the outer rect
bool rect_engulfs(Rect outer, Rect inner);
//Check if a rect contains a point
bool rect_contains(Rect r, Vector2 point);
//Check to see if a rect intersects a line
bool rect_intersects(Rect r, Line l);
//Check if two rectangles overlap
bool rect_overlaps_rect(Rect a, Rect b);
//Check if a rectangle overlaps a circle
bool rect_overlaps_circ(Rect r, Circ c);
