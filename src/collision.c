#include "collision.h"

bool engulfs_circ(Circ outer, Circ inner);
bool engulfs_rect(Rect outer, Rect inner) {
	return inner.x >= outer.x && inner.x + inner.width <= outer.x + outer.width && 
		inner.y >= outer.y && inner.y + inner.height <= outer.y + outer.height;
}
bool engulfs_rect_circ(Rect outer, Circ inner) {
	return engulfs_rect(outer, circ_bounding_box(inner));
}
bool engulfs_circ_rect(Circ outer, Rect inner) {
	return engulfs_rect(circ_bounding_box(outer), inner);
}
bool overlaps_circ(Circ a, Circ b) {
	Vector2 center = vec2_new(a.x, a.y);
	Vector2 other = vec2_new(b.x, b.y);
	float radius_sum = a.radius + b.radius;
	return vec2_len2(vec2_sub(center, other)) < (radius_sum * radius_sum);
}
bool overlaps_rect(Rect a, Rect b) {
	return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}
bool overlaps_rect_circ(Rect r, Circ c) {
	return circ_intersects(c, rect_left(r)) || circ_intersects(c, rect_right(r)) || circ_intersects(c, rect_top(r)) || circ_intersects(c, rect_bottom(r));
}
#define POLY_OVERLAP(poly, other, check_line) \
	for(size_t i = 0; i < poly.num_points; i++) { \
		Vector2 one = poly_get_vertex(poly, i % poly.num_points); \
		Vector2 two = poly_get_vertex(poly, (i + poly.num_points - 1) % poly.num_points); \
		Line face = line_new(one, two); \
		if(check_line(other, face)) { \
			return true; \
		} \
	} \
	return false;
bool overlaps_poly(Polygon a, Polygon b) {
	POLY_OVERLAP(a, b, poly_intersects);
}
bool overlaps_poly_rect(Polygon a, Rect b) {
	POLY_OVERLAP(a, b, rect_intersects);
}
bool overlaps_poly_circ(Polygon a, Circ b) {
	POLY_OVERLAP(a, b, circ_intersects);
}
bool overlaps_shape(Shape a, Shape b) {
	if(a.type == b.type) {
		switch(a.type) {
			case SHAPE_IS_CIRC:
				return overlaps_circ(a.data.c, b.data.c);
			case SHAPE_IS_POLY:
				return overlaps_poly(a.data.p, b.data.p);
			case SHAPE_IS_RECT:
				return overlaps_rect(a.data.r, b.data.r);
		}
	} else {
		switch(a.type) {
			case SHAPE_IS_CIRC:
				return overlaps_shape(b, a);
			case SHAPE_IS_RECT:
				switch(b.type) {
					case SHAPE_IS_CIRC:
						return overlaps_rect_circ(a.data.r, b.data.c);
					case SHAPE_IS_RECT:
						return overlaps_rect(a.data.r, b.data.r);
					case SHAPE_IS_POLY:
						return overlaps_poly_rect(b.data.p, a.data.r);
				}
			case SHAPE_IS_POLY:
				switch(b.type) {
					case SHAPE_IS_CIRC:
						return overlaps_poly_circ(a.data.p, b.data.c);
					case SHAPE_IS_RECT:
						return overlaps_poly_rect(a.data.p, b.data.r);
					case SHAPE_IS_POLY:
						return overlaps_poly(a.data.p, b.data.p);
				}
		}
	}
	return false;
}
