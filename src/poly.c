#include "poly.h"
#include <stdlib.h>

//Create a new polygon from a list of vertices
Polygon poly_new(Vector2 position, Vector2 *points, size_t num_points) {
	return (Polygon) {position, vec2_new(0.f, 0.f), points, num_points, 0.f, 0.f};
}
//Create a new polygon from a rectangle
Polygon poly_from_rect(Rect rect) {
	Vector2 *points = malloc(sizeof(*points) * 4);
	points[0] = vec2_new(0.f, 0.f);
	points[1] = vec2_new(rect.width, 0.f);
	points[2] = vec2_new(rect.width, rect.height);
	points[3] = vec2_new(0.f, rect.height);
	return poly_new(vec2_new(rect.x, rect.y), points, 4);
}
//Get a transformed vertex from a polygon
Vector2 poly_get_vertex(Polygon poly, size_t index) {
	Vector2 original = poly.points[index];
	Vector2 translated = vec2_sub(original, poly.origin);
	translated = vec2_rotate(translated, poly.rotation);
	translated = vec2_scl(translated, poly.scale);
	translated = vec2_add(translated, poly.origin);
	return vec2_add(translated, poly.pos);
}
//Find if a polygon contains a point
bool poly_contains(Polygon poly, Vector2 point) {
	size_t i = 0, j = poly.num_points - 1;
	bool c = 0;
	while(i < poly.num_points) {
		Vector2 vertexI = poly_get_vertex(poly, i);
		Vector2 vertexJ = poly_get_vertex(poly, j);
		if((vertexI.y > point.y) != (vertexJ.y > point.y) && 
				(point.x < (vertexJ.x - vertexI.x) * (point.y - vertexI.y) / (vertexJ.y - vertexI.y) + vertexI.x)) {
			c = !c; 
		}
		j = i;
		i += 1;
	}
	return c;
}
//Find if a polygon collided with a line
bool poly_intersects(Polygon poly, Line line) {
	for(size_t i = 0; i < poly.num_points; i++) {
		Vector2 one = poly_get_vertex(poly, i % poly.num_points);
		Vector2 two = poly_get_vertex(poly, (i + poly.num_points - 1) % poly.num_points);
		Line face = line_new(one, two);
		if(line_intersects(face, line)) {
			return true;
		}
	}
	return false;
}
void poly_destroy(Polygon poly) {
	free(poly.points);
}
