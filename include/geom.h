#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * \file geom.h
 * Defines basic math and geometry structures for collision and simulation.
 */

///A circle structure
typedef struct Circ {
	float x, y, radius;
} Circ;
///A 2D Axis-Aligned Bounding Box
typedef struct Rect {
	float x, y, width, height;
} Rect;
///A 2D vector
typedef struct Vector2 {
	float x, y;
} Vector2; 
///A line segment
typedef struct Line {
	Vector2 start, end;
} Line;
///A 2D n-sided polygon
typedef struct Polygon {
	Vector2 pos; ///The displacement of the shape
	Vector2 origin; ///The rotational origin of the shape
	Vector2 *points; ///The buffer of the points in the polygon
	Vector2 *transformed; ///The cached version of the points
	size_t num_points; ///The number of points in the buffer
	float rotation; ///The degrees of rotation of the shape
	float scale; ///The scale factor of the shape
} Polygon;
///A generic handle for a 2D shape
typedef struct Shape {
	union {
		Rect r;
		Circ c;
		Polygon p;
	} data; ///Store the shape types in a union
	enum { SHAPE_IS_RECT, SHAPE_IS_CIRC, SHAPE_IS_POLY} type; ///The type of shape it's storing
	float rot; ///Store the rotation
} Shape;

// *** COLLISION FUNCTIONS ***
/// Checks to see if a Circ is entirely contained within another
bool engulfs_circ(Circ outer, Circ inner); // TODO : Implement
/// Checks to see if a Rect is entirely contained within another
bool engulfs_rect(Rect outer, Rect inner);
/// Checks to see if a Circ is entirely contained within a Rect
bool engulfs_rect_circ(Rect outer, Circ inner);
/// Checks to see if a Rect is entirely contaiend within a Circ
bool engulfs_circ_rect(Circ outer, Rect inner);
/// Checks to see if two Circ s bound any of the same area
bool overlaps_circ(Circ a, Circ b);
/// Checks to see if two Polygon s bound any of the same area
bool overlaps_poly(Polygon a, Polygon b);
/// Checks to see if two Rect s bound any of the same area
bool overlaps_rect(Rect a, Rect b);
/// Checks to see if a Rect and Circ bound any of the same area
bool overlaps_rect_circ(Rect a, Circ b);
/// Checks to see if a Polygon and Rect bound any of the same area
bool overlaps_poly_rect(Polygon a, Rect b);
/// Checks to see if a Polygon and Circ bound any of the same area
bool overlaps_poly_circ(Polygon a, Circ b);
/// Checks to see if two Shape s bound any of the same area
bool overlaps_shape(Shape a, Shape b);

// *** VECTORS ***
///Create a new vector with an x and a y 
Vector2 vec2_new(float x, float y);
///Find the square of the length of a vector
float vec2_len2(Vector2 vec);
///Find the length of a vector
float vec2_len(Vector2 vec);
///Dot product two vectors
float vec2_dot(Vector2 a, Vector2 b);
///Cross product two vectors
float vec2_cross(Vector2 a, Vector2 b);
///Normalize a vector
Vector2 vec2_nor(Vector2 vec);
///Scale a vector
Vector2 vec2_scl(Vector2 vec, float scalar);
///Set the length of the vector
Vector2 vec2_with_len(Vector2 vec, float len);
///Rotate a vector 
Vector2 vec2_rotate(Vector2 vec, float angle_in_degrees);
///Add two vectors
Vector2 vec2_add(Vector2 a, Vector2 b);
///Subtract two vectors
Vector2 vec2_sub(Vector2 a, Vector2 b);
///Invert a vector2
Vector2 vec2_neg(Vector2 vec);
///Check if two vectors are equal
bool vec2_eql(Vector2 a, Vector2 b);
///Get the angle a vector makes with the x-axis
float vec2_angle(Vector2 a);


// *** LINES ***
///Create a new line between two points
Line line_new(Vector2 start, Vector2 end);
///Check if two lines intersect; the end points are included
bool line_intersects(Line a, Line b);
///Check if two lines are equal
bool line_eql(Line a, Line b);

// *** CIRCLES ***
///Create a new circle
Circ circ_new(float x, float y, float radius);
///Check if a point is in a circle
bool circ_contains(Circ c, Vector2 v);
///Check if a circle intersects a line
bool circ_intersects(Circ c, Line l);
///Get the rect bounding box of a circle
Rect circ_bounding_box(Circ circ);


// *** RECTANGLES ***
///Create a new rectangle
Rect rect_new(float x, float y, float width, float height);
///Get the left side of the rect
Line rect_left(Rect r);
///Get the right side of the rect
Line rect_right(Rect r);
///Get the top of the rect
Line rect_top(Rect r);
///Get the bottom of the rect
Line rect_bottom(Rect r);
///Check if a rect contains a point
bool rect_contains(Rect r, Vector2 point);
///Check to see if a rect intersects a line
bool rect_intersects(Rect r, Line l);

// *** POLYGONS ***
///Create a new polygon from a list of vertices
Polygon poly_new(Vector2 position, Vector2 *points, size_t num_points);
///Create a new polygon from a rectangle
Polygon poly_from_rect(Rect rect);
///Get a transformed vertex from a polygon
Vector2 poly_get_vertex(Polygon poly, size_t index);
///Find if a polygon contains a point
bool poly_contains(Polygon poly, Vector2 point);
///Find if a polygon collided with a line
bool poly_intersects(Polygon poly, Line line);
///Find the bounds of a polygon as a rect
Rect poly_bounding_box(Polygon poly);
///Update the points of the polygon
void poly_update(Polygon poly);
///Deallocate the polygon
void poly_destroy(Polygon poly);

// *** SHAPES ***
///Create a shape from a rectangle
Shape shape_rect(Rect r);
///Create a shape from a circle
Shape shape_circ(Circ c);
///Create a shape from a polygon
Shape shape_poly(Polygon p);
///Set the shape's top-left corner
void shape_set_position(Shape *s, Vector2 position);
///Get the position of a shape's top-left corner
Vector2 shape_get_position(Shape s);
///Checks to see if a shape contains a point
bool shape_contains(Shape s, Vector2 point);
///Checks to see if a shape bounds or intersects a line
bool shape_intersects(Shape s, Line line);
///Get the smallest rectangle that fully envelpos the shape
Rect shape_bounding_box(Shape s);
///Get the rotation of a Shape
float shape_get_rotation(Shape s);
///Set the rotation of a Shape; a Shape backed by a Rect will be converted into a Polygon
void shape_set_rotation(Shape *s, float rotation);
///Rotates a shape by a given amount; a Shape backed by a Rect will be converted into a Polygon
void shape_rotate(Shape *s, float amount);
///Get the center of a shape
Vector2 shape_get_center(Shape s);
