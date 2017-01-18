#include <assert.h>
#include "circ.h"
#include "collision.h"
#include "lines.h"
#include "rect.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "quadtree.h"

void vector_length() {
	Vector2 vec = vec2_new(3.f, 4.f);
	assert(vec2_len2(vec) == 25.f);
	assert(vec2_len(vec) == 5.f);
}

void vector_scale() {
	Vector2 vec = vec2_new(10.f, 10.f);
	Vector2 new_len = vec2_with_len(vec, 5.f);
	assert(vec2_len(new_len) == 5.f);
	Vector2 long_vector = vec2_new(10.f, 0.f);
	Vector2 unit = vec2_nor(long_vector);
	assert(vec2_len(unit) == 1.f);
	assert(unit.x == 1.f);
	Vector2 doubled = vec2_scl(unit, 2.f);
	assert(vec2_len(doubled) == 2.f);
	assert(doubled.x == 2.f);
}

void vector_arithmetic() {
	Vector2 a = vec2_new(4.f, 0.f);
	Vector2 b = vec2_new(1.f, -3.f);
	Vector2 c = vec2_new(5.f, -3.f);
	assert(vec2_eql(vec2_add(a, b), c));
	assert(!vec2_eql(vec2_sub(a, b), c));
	assert(vec2_eql(vec2_sub(c, a), b));
	assert(vec2_eql(vec2_sub(c, b), a));
}

void vector_rotation() {
	Vector2 a = vec2_new(1.f, 0.f);
	Vector2 b = vec2_new(0.f, 1.f);
	assert(vec2_eql(vec2_rotate(a, 90.f), b));
	assert(vec2_eql(vec2_rotate(a, -270.f), b));
	assert(vec2_eql(vec2_rotate(b, -90.f), a));
	assert(vec2_eql(vec2_rotate(b, 270.f), a));
	assert(vec2_eql(vec2_rotate(a, 0.f), a));
	assert(vec2_eql(vec2_rotate(b, 0.f), b));
}

void line_intersect() {
	Vector2 a = vec2_new(0.f, 0.f);
	Vector2 b = vec2_new(5.1f, 5.1f);
	Vector2 c = vec2_new(-1.f, -2.f);
	Vector2 d = vec2_new(5.f, -2.f);
	Vector2 e = vec2_new(5.f, 6.f);
	Line l1 = line_new(a, b);
	Line l2 = line_new(a, c);
	Line l3 = line_new(d, e);
	Line l4 = line_new(c, b);
	assert(line_intersects(l1,l1));
	assert(line_intersects(l1,l2));
	assert(!line_intersects(l2,l3));
	assert(line_intersects(l1,l3));
	assert(line_intersects(l3,l4));
	assert(line_intersects(l2,l1));
	assert(!line_intersects(l3,l2));
	assert(line_intersects(l3,l1));
	assert(line_intersects(l4,l3));
}

void rect_intersect() {
	Line t = line_new(vec2_new(0.f, 0.f), vec2_new(32.f, 0.f));
	Line bot = line_new(vec2_new(0.f, 32.f), vec2_new(32.f, 32.f));
	Line r = line_new(vec2_new(32.f, 0.f), vec2_new(32.f, 32.f));
	Line l = line_new(vec2_new(0.f, 0.f), vec2_new(0.f, 32.f));
	Rect rect = rect_new(0.f, 0.f, 32.f, 32.f);
	assert(line_eql(t, rect_top(rect)));
	assert(line_eql(bot, rect_bottom(rect)));
	assert(line_eql(r, rect_right(rect)));
	assert(line_eql(l, rect_left(rect)));
}

void rect_engulf_test() {
	Rect a = rect_new(0.f, 0.f, 64.f, 64.f);
	Rect b = rect_new(0.f, 0.f, 32.f, 32.f);
	Rect c = rect_new(0.f, 0.f, 16.f, 16.f);
	assert(engulfs_rect(a,b));
	assert(engulfs_rect(a,c));
	assert(engulfs_rect(b,c));
	assert(!engulfs_rect(b,a));
	assert(!engulfs_rect(c,a));
	assert(!engulfs_rect(c,b));
}

void rect_overlaps_test() {
	Rect a = rect_new(0.f, 0.f, 5.f, 5.f);
	Rect b = rect_new(6.f, 6.f, 5.f, 5.f);
	Rect c = rect_new(2.f, 2.f, 10.f, 10.f);
	Rect d = rect_new(1.f, 1.f, 1.f, 1.f);
	assert(!overlaps_rect(a,b));
	assert(!overlaps_rect(b,a));
	assert(overlaps_rect(a,c));
	assert(overlaps_rect(a,d));
	assert(overlaps_rect(c,a));
	assert(overlaps_rect(b,c));
	assert(overlaps_rect(c,b));
	assert(overlaps_rect(d,a));
}

void circle_overlaps_test() {
	Circ a = circ_new(0.f, 0.f, 5.f);
	Circ b = circ_new(10.f, 10.f, 2.f);
	Circ c = circ_new(5.f, 5.f, 32.f);
	assert(!overlaps_circ(a, b));
	assert(!overlaps_circ(b, a));
	assert(overlaps_circ(a, c));
	assert(overlaps_circ(c, a));
	assert(overlaps_circ(b, c));
	assert(overlaps_circ(c, b));
}

void rect_circ_overlaps_test() {
	Rect a = rect_new(0.f, 0.f, 32.f, 32.f); 
	Circ b = circ_new(0.f, 0.f, 5.f);
	Rect c = rect_new(-10.f, -20.f, 1.f, 5.f);
	assert(overlaps_rect_circ(a, b));
	assert(!overlaps_rect_circ(c, b));
}

#undef main
int main(int argc, char *argv[]) {
	if(argc < 2) {
		puts("Enter a test name.");
		return 2;
	}
	if(strcmp(argv[1], "veclen") == 0) {
		vector_length();
	} else if(strcmp(argv[1], "vecscl") == 0) {
		vector_scale();
	} else if(strcmp(argv[1], "vecari") == 0) {
		vector_arithmetic();
	} else if(strcmp(argv[1], "vecrot") == 0) {
		vector_rotation();
	} else if(strcmp(argv[1], "lineint") == 0) {
		line_intersect();
	} else if(strcmp(argv[1], "rectint") == 0) {
		rect_intersect();
	} else if(strcmp(argv[1], "recteng") == 0) {
		rect_engulf_test();
	} else if(strcmp(argv[1], "rectovr") == 0) {
		rect_overlaps_test();
	} else if(strcmp(argv[1], "circovr") == 0) {
		circle_overlaps_test();
	} else if(strcmp(argv[1], "rectcirc") == 0) {
		rect_circ_overlaps_test();
	} else {
		return 1;
		puts("No such test.\n");
	}
	puts("Test succesful.\n");
	return 0;
}
