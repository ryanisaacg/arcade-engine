#include "lines.h"
#include "rect.h"

//Create a new recangle
Rect rect_new(float x, float y, float width, float height) {
	return (Rect){x, y, width, height};
}
//Get the left side of the rect
Line rect_left(Rect r) {
	return line_new(vec2_new(r.x, r.y), vec2_new(r.x, r.y + r.height));
}
//Get the right side of the rect
Line rect_right(Rect r) {
	return line_new(vec2_new(r.x + r.width, r.y), vec2_new(r.x + r.width, r.y + r.height));
}
//Get the top of the rect
Line rect_top(Rect r) {
	return line_new(vec2_new(r.x, r.y), vec2_new(r.x + r.width, r.y));
}
//Get the bottom of the rect
Line rect_bottom(Rect r) {
	return line_new(vec2_new(r.x, r.y + r.height), vec2_new(r.x + r.width, r.y + r.height));
}
//Check if the inner rect is entirely contained within the outer rect
bool rect_engulfs(Rect outer, Rect inner) {
	return inner.x >= outer.x && inner.x + inner.width <= outer.x + outer.width && 
		inner.y >= outer.y && inner.y + inner.height <= outer.y + outer.height;
}
//Check if a rect contains a point
bool rect_contains(Rect r, Vector2 point) {
	return point.x >= r.x && point.y >= r.y && point.x < r.x + r.width && point.y < r.y + r.height;
}
//Check to see if a rect intersects a line
bool rect_intersects(Rect r, Line line) {
	return rect_contains(r, line.start) || rect_contains(r, line.end) || line_intersects(rect_left(r), line) ||
		line_intersects(rect_top(r), line) || line_intersects(rect_right(r), line) || line_intersects(rect_bottom(r), line);
}
//Check if two rectangles overlap
bool rect_overlaps_rect(Rect a, Rect b) {
	return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}
