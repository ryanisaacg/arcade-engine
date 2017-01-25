#pragma once

#include <stdbool.h>

#include "forward-decl.h"

//A 2D vector
struct Vector2 {
	float x, y;
};
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
struct Line {
	Vector2 start, end;
};
//Create a new line between two points
Line line_new(Vector2 start, Vector2 end);
//Check if two lines intersect
//The end points are included
bool line_intersects(Line a, Line b);
//Check if two lines are equal
bool line_eql(Line a, Line b);
