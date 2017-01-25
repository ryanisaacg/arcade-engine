#pragma once

#include "types.h"

// *** COLLISION FUNCTIONS ***
// Engulf checks to see if all of the shape is inside the other
bool engulfs_circ(Circ outer, Circ inner); // TODO : Implement
bool engulfs_rect(Rect outer, Rect inner);
bool engulfs_rect_circ(Rect outer, Circ inner);
bool engulfs_circ_rect(Circ outer, Rect inner);
// Overlaps checks to see if any of the two shapes bound the same area
bool overlaps_circ(Circ a, Circ b);
bool overlaps_poly(Polygon a, Polygon b);
bool overlaps_rect(Rect a, Rect b);
bool overlaps_rect_circ(Rect a, Circ b);
bool overlaps_poly_rect(Polygon a, Rect b);
bool overlaps_poly_circ(Polygon a, Circ b);
bool overlaps_shape(Shape a, Shape b);

// *** VECTORS ***
//Create a new vector with an x and a y 
Vector2 vec2_new(float x, float y);
//Find the square of the length of a vector
float vec2_len2(Vector2 vec);
//Find the length of a vector
float vec2_len(Vector2 vec);
//Dot product two vectors
float vec2_dot(Vector2 a, Vector2 b);
//Cross product two vectors
float vec2_cross(Vector2 a, Vector2 b);
//Normalize a vector
Vector2 vec2_nor(Vector2 vec);
//Scale a vector
Vector2 vec2_scl(Vector2 vec, float scalar);
//Set the length of the vector
Vector2 vec2_with_len(Vector2 vec, float len);
//Rotate a vector 
Vector2 vec2_rotate(Vector2 vec, float angle_in_degrees);
//Add two vectors
Vector2 vec2_add(Vector2 a, Vector2 b);
//Subtract two vectors
Vector2 vec2_sub(Vector2 a, Vector2 b);
//Invert a vector2
Vector2 vec2_neg(Vector2 vec);
//Check if two vectors are equal
bool vec2_eql(Vector2 a, Vector2 b);
//Get the angle a vector makes with the x-axis
float vec2_angle(Vector2 a);


// *** LINES ***
//Create a new line between two points
Line line_new(Vector2 start, Vector2 end);
//Check if two lines intersect
//The end points are included
bool line_intersects(Line a, Line b);
//Check if two lines are equal
bool line_eql(Line a, Line b);

// *** CIRCLES ***
//Create a new circle
Circ circ_new(float x, float y, float radius);
//Check if a point is in a circle
bool circ_contains(Circ c, Vector2 v);
//Check if a circle intersects a line
bool circ_intersects(Circ c, Line l);
//Get the rect bounding box of a circle
Rect circ_bounding_box(Circ circ);


// *** RECTANGLES ***
//Create a new rectangle
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

// *** POLYGONS ***
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

// *** SHAPES ***
//Create a shape
Shape shape_rect(Rect r);
Shape shape_circ(Circ c);
Shape shape_poly(Polygon p);
//Handle the shape top-left corner
void shape_set_position(Shape *s, Vector2 position);
Vector2 shape_get_position(Shape s);
//Shape collision functions
bool shape_contains(Shape s, Vector2 point);
bool shape_intersects(Shape s, Line line);
//Get the smallest rectangle that fully envelpos the shape
Rect shape_bounding_box(Shape s);
//Handle shape rotation. Rotated rectangles become Polygons
float shape_get_rotation(Shape s);
void shape_set_rotation(Shape *s, float rotation);
void shape_rotate(Shape *s, float amount);
Vector2 shape_get_center(Shape s);
