#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * \file geom.h
 * \brief Defines basic math and geometry structures for collision and simulation
 *
 * geom.h contains definitions for Circ, Rect, and Polygon shapes, as well as a Shape handle that unifies them all. 
 * It also contains defintions for Vector2 and Line, for vectors and lines, respectively. 
 * Functions are defined to check if a point is contained in a shape, if a line is contained in a shape, or a shape intersects another shape.
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
	/**
	 * \brief The offset of the Polygon
	 *
	 * Polygon points are stored assuming the shape is anchored at 0,0. The pos member sets where the Polygon is in space.
	 */
	Vector2 pos; 
	/**
	 * \brief The rotational origin of the shape
	 *
	 * To achieve a spinning behavior when rotating, the origin of a Polygon should be placed at its center.
	 */
	Vector2 origin; 
	/**
	 * \private
	 * The buffer of the points in the Polygon
	 */
	Vector2 *points;
	/**
	 * \private
	 * The cached version of the transformed points
	 */
	Vector2 *transformed;
	/**
	 * \private
	 * The number of points in the Polygon buffers
	 */
	size_t num_points;
	/**
	 * \brief The rotation of the shape, given in degrees
	 * 
	 * Rotation is applied relative to the Polygon 's origin
	 */
	float rotation;
	/**
	 * \brief The scale factor of the shape, where 1 is no scaling
	 *
	 * Multiplier uniformly applied to all points in the Polygon. 0 will not necessarily produce sensible results. 
	 */
	float scale;
} Polygon;
/**
 * \brief A generic handle for a 2D shape
 * 
 * Uses a union that combines the Rect, Circle, and Polygon structures
 */
typedef struct Shape {
	//Store the shape types in a union
	/// \private
	union {
		Rect r;
		Circ c;
		Polygon p;
	} data; 
	///The type of shape it's storing
	/// \private
	enum { SHAPE_IS_RECT, SHAPE_IS_CIRC, SHAPE_IS_POLY} type; 
	//The rotation of the shape
	/// \private
	float rot; 
} Shape;

// *** COLLISION FUNCTIONS ***
/// \brief Checks to see if a Circ is entirely contained within another
///
/// \TODO: Implement
bool engulfs_circ(const Circ outer, const Circ inner);
/// Checks to see if a const Rect is entirely contained within another
bool engulfs_rect(const Rect outer, const Rect inner);
/// Checks to see if a const Circ is entirely contained within a const Rect
bool engulfs_rect_circ(const Rect outer, const Circ inner);
/// Checks to see if a const Rect is entirely contaiend within a const Circ
bool engulfs_circ_rect(const Circ outer, const Rect inner);
/// Checks to see if two const Circ s bound any of the same area
bool overlaps_circ(const Circ a, const Circ b);
/// Checks to see if two const Polygon s bound any of the same area
bool overlaps_poly(const Polygon a, const Polygon b);
/// Checks to see if two const Rect s bound any of the same area
bool overlaps_rect(const Rect a, const Rect b);
/// Checks to see if a const Rect and const Circ bound any of the same area
bool overlaps_rect_circ(const Rect a, const Circ b);
/// Checks to see if a const Polygon and const Rect bound any of the same area
bool overlaps_poly_rect(const Polygon a, const Rect b);
/// Checks to see if a const Polygon and const Circ bound any of the same area
bool overlaps_poly_circ(const Polygon a, const Circ b);
/// Checks to see if two const Shape s bound any of the same area
bool overlaps_shape(const Shape a, const Shape b);

// *** VECTORS ***
///Create a new vector with an x and a y 
Vector2 vec2_new(const float x, const float y);
///Find the square of the length of a vector
float vec2_len2(const Vector2 vec);
///Find the length of a vector
float vec2_len(const Vector2 vec);
///Dot product two vectors
float vec2_dot(const Vector2 a, const Vector2 b);
///Cross product two vectors
float vec2_cross(const Vector2 a, const Vector2 b);
///Normalize a vector
Vector2 vec2_nor(const Vector2 vec);
///Scale a vector
Vector2 vec2_scl(const Vector2 vec, const float scalar);
///Set the length of the vector
Vector2 vec2_with_len(const Vector2 vec, const float len);
///Rotate a vector 
Vector2 vec2_rotate(const Vector2 vec, const float angle_in_degrees);
///Add two vectors
Vector2 vec2_add(const Vector2 a, const Vector2 b);
///Subtract two vectors
Vector2 vec2_sub(const Vector2 a, const Vector2 b);
///Invert a vector2
Vector2 vec2_neg(const Vector2 vec);
///Check if two vectors are equal
bool vec2_eql(const Vector2 a, const Vector2 b);
///Get the angle a vector makes with the x-axis
float vec2_angle(const Vector2 a);


// *** LINES ***
///Create a new line between two points
Line line_new(const Vector2 start, const Vector2 end);
///Check if two lines intersect; the end points are included
bool line_intersects(const Line a, const Line b);
///Check if two lines are equal
bool line_eql(const Line a, const Line b);

// *** CIRCLES ***
///Create a new circle
Circ circ_new(const float x, const float y, const float radius);
///Check if a point is in a circle
bool circ_contains(const Circ c, const Vector2 v);
///Check if a circle intersects a line
bool circ_intersects(const Circ c, const Line l);
///Get the rect bounding box of a circle
Rect circ_bounding_box(const Circ circ);


// *** RECTANGLES ***
///Create a new rectangle
Rect rect_new(const float x, const float y, const float width, const float height);
///Get the left side of the rect
Line rect_left(const Rect r);
///Get the right side of the rect
Line rect_right(const Rect r);
///Get the top of the rect
Line rect_top(const Rect r);
///Get the bottom of the rect
Line rect_bottom(const Rect r);
///Check if a rect contains a point
bool rect_contains(const Rect r, const Vector2 point);
///Check to see if a rect intersects a line
bool rect_intersects(const Rect r, const Line l);

// *** POLYGONS ***
///Create a new polygon from a list of vertices
///
///Consider 'points' to be now owned by the Polygon; when poly_destroy is called it will be freed
Polygon poly_new(const Vector2 position, Vector2 *points, const size_t num_points);
///Create a new polygon from a rectangle
Polygon poly_from_rect(const Rect rect);
///Get a transformed vertex from a polygon
Vector2 poly_get_vertex(const Polygon poly, const size_t index);
///Find if a polygon contains a point
bool poly_contains(const Polygon poly, const Vector2 point);
///Find if a polygon collided with a line
bool poly_intersects(const Polygon poly, const Line line);
///Find the bounds of a polygon as a rect
Rect poly_bounding_box(const Polygon poly);
///Update the points of the polygon
void poly_update(Polygon poly);
///Deallocate the polygon
void poly_destroy(const Polygon poly);

// *** SHAPES ***
///Create a shape from a rectangle
Shape shape_rect(const Rect r);
///Create a shape from a circle
Shape shape_circ(const Circ c);
///Create a shape from a polygon
Shape shape_poly(const Polygon p);
///Set the shape's top-left corner
void shape_set_position(Shape *s, const Vector2 position);
///Get the position of a shape's top-left corner
Vector2 shape_get_position(const Shape s);
///Checks to see if a shape contains a point
bool shape_contains(const Shape s, const Vector2 point);
///Checks to see if a shape bounds or intersects a line
bool shape_intersects(const Shape s, const Line line);
///Get the smallest rectangle that fully envelpos the shape
Rect shape_bounding_box(const Shape s);
///Get the rotation of a const Shape
float shape_get_rotation(const Shape s);
///Set the rotation of a const Shape; a const Shape backed by a const Rect will be converted into a const Polygon
void shape_set_rotation(Shape *s, const float rotation);
///Rotates a shape by a given amount; a const Shape backed by a const Rect will be converted into a const Polygon
void shape_rotate(Shape *s, const float amount);
///Get the center of a shape
Vector2 shape_get_center(const Shape s);
