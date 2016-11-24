#include "collision.h"

bool engulfs_circ(Circ outer, Circ inner);
bool engulfs_rect(Rect outer, Rect inner) {
	return inner.x >= outer.x && inner.x + inner.width <= outer.x + outer.width && 
		inner.y >= outer.y && inner.y + inner.height <= outer.y + outer.height;
}
bool engulfs_rect_circ(Rect outer, Circ inner);
bool engulfs_circ_rect(Circ outer, Rect inner);
bool overlap_circ(Circ a, Circ b) {
	Vector2 center = vec2_new(a.x, a.y);
	Vector2 other = vec2_new(b.x, b.y);
	float radius_sum = a.radius + b.radius;
	return vec2_len2(vec2_sub(center, other)) < (radius_sum * radius_sum);
}
bool overlap_poly(Polygon a, Polygon b);
bool overlap_rect(Rect a, Rect b) {
	return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}
bool overlap_rect_circ(Rect r, Circ c) {
	return circ_intersects(c, rect_left(r)) || circ_intersects(c, rect_right(r)) || circ_intersects(c, rect_top(r)) || circ_intersects(c, rect_bottom(r));
}
bool overlap_poly_rect(Polygon a, Rect b);
bool overlap_poly_circ(Polygon a, Circ b);
