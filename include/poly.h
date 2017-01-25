#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "forward-decl.h"

struct Polygon{
	Vector2 pos, origin;
	Vector2 *points;
	size_t num_points;
	float rotation, scale;
};

#include "lines.h"
#include "rect.h"

//Create a new polygon from a list of vertices
Polygon poly_new(Vector2 position, Vector2 *points, size_t num_points);
//Create a new polygon from a rectangle
Polygon poly_from_rect(Rect rect);
//Get a transformed vertex from a polygon
Vector2 poly_get_vertex(Polygon poly, size_t index);
//Find if a polygon contains a point
bool poly_contains(Polygon poly, Vector2 point);
//Find if a polygon collided with a line
bool poly_intersects(Polygon poly, Line line);
//Find the bounds of a polygon as a rect
Rect poly_bounding_box(Polygon poly);
void poly_destroy(Polygon poly);
