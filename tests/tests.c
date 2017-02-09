#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "arcade.h"
#include "framework.h"

char *vector_length() {
	Vector2 vec = vec2_new(3.f, 4.f);
	assert("Vector2 Length 1", vec2_len2(vec) == 25.f);
	assert("Vector2 Length 2", vec2_len(vec) == 5.f);
}

char *vector_scale() {
	Vector2 vec = vec2_new(10.f, 10.f);
	Vector2 new_len = vec2_with_len(vec, 5.f);
	assert("Vector2 with length", vec2_len(new_len) == 5.f);
	Vector2 long_vector = vec2_new(10.f, 0.f);
	Vector2 unit = vec2_nor(long_vector);
	assert("Normalizing vectors", vec2_len(unit) == 1.f);
	assert("Normalizing vectors 2", unit.x == 1.f);
	Vector2 doubled = vec2_scl(unit, 2.f);
	assert("Scaling vectors", vec2_len(doubled) == 2.f);
	assert("Scaling vectors 2", doubled.x == 2.f);
}

char *vector_arithmetic() {
	Vector2 a = vec2_new(4.f, 0.f);
	Vector2 b = vec2_new(1.f, -3.f);
	Vector2 c = vec2_new(5.f, -3.f);
	assert("Vector adding", vec2_eql(vec2_add(a, b), c));
	assert("Vector subtracting", !vec2_eql(vec2_sub(a, b), c));
	assert("Vector subtracting 2", vec2_eql(vec2_sub(c, a), b));
	assert("Vector2 subtracting 3", vec2_eql(vec2_sub(c, b), a));
}

char *vector_rotation() {
	Vector2 a = vec2_new(1.f, 0.f);
	Vector2 b = vec2_new(0.f, 1.f);
	assert("Vector rotation 1", vec2_eql(vec2_rotate(a, 90.f), b));
	assert("Vector rotation 2", vec2_eql(vec2_rotate(a, -270.f), b));
	assert("Vector rotation 3", vec2_eql(vec2_rotate(b, -90.f), a));
	assert("Vector rotation 4", vec2_eql(vec2_rotate(b, 270.f), a));
	assert("Vector rotation 5", vec2_eql(vec2_rotate(a, 0.f), a));
	assert("Vector rotation 6", vec2_eql(vec2_rotate(b, 0.f), b));
}

char *line_intersect() {
	Vector2 a = vec2_new(0.f, 0.f);
	Vector2 b = vec2_new(5.1f, 5.1f);
	Vector2 c = vec2_new(-1.f, -2.f);
	Vector2 d = vec2_new(5.f, -2.f);
	Vector2 e = vec2_new(5.f, 6.f);
	Line l1 = line_new(a, b);
	Line l2 = line_new(a, c);
	Line l3 = line_new(d, e);
	Line l4 = line_new(c, b);
	assert("Line intersect 1", line_intersects(l1,l1));
	assert("Line intersect 2", line_intersects(l1,l2));
	assert("Line intersect 3", !line_intersects(l2,l3));
	assert("Line intersect 4", line_intersects(l1,l3));
	assert("Line intersect 5", line_intersects(l3,l4));
	assert("Line intersect 6", line_intersects(l2,l1));
	assert("Line intersect 7", !line_intersects(l3,l2));
	assert("Line intersect 8", line_intersects(l3,l1));
	assert("Line intersect 9", line_intersects(l4,l3));
}

char *rect_intersect() {
	Line t = line_new(vec2_new(0.f, 0.f), vec2_new(32.f, 0.f));
	Line bot = line_new(vec2_new(0.f, 32.f), vec2_new(32.f, 32.f));
	Line r = line_new(vec2_new(32.f, 0.f), vec2_new(32.f, 32.f));
	Line l = line_new(vec2_new(0.f, 0.f), vec2_new(0.f, 32.f));
	Rect rect = rect_new(0.f, 0.f, 32.f, 32.f);
	assert("Rectangle line retrieval top", line_eql(t, rect_top(rect)));
	assert("Rectangle line retrieval bottom", line_eql(bot, rect_bottom(rect)));
	assert("Rectangle line retrieval right" ,line_eql(r, rect_right(rect)));
	assert("Rectangle line retrieval left", line_eql(l, rect_left(rect)));
}

char *rect_engulf_test() {
	Rect a = rect_new(0.f, 0.f, 64.f, 64.f);
	Rect b = rect_new(0.f, 0.f, 32.f, 32.f);
	Rect c = rect_new(0.f, 0.f, 16.f, 16.f);
	assert("Engulf rect 1", engulfs_rect(a,b));
	assert("Engulf rect 2", engulfs_rect(a,c));
	assert("Engulf rect 3", engulfs_rect(b,c));
	assert("Engulf rect 4", !engulfs_rect(b,a));
	assert("Engulf rect 5", !engulfs_rect(c,a));
	assert("Engulf rect 6", !engulfs_rect(c,b));
}

char *rect_overlaps_test() {
	Rect a = rect_new(0.f, 0.f, 5.f, 5.f);
	Rect b = rect_new(6.f, 6.f, 5.f, 5.f);
	Rect c = rect_new(2.f, 2.f, 10.f, 10.f);
	Rect d = rect_new(1.f, 1.f, 1.f, 1.f);
	assert("Rect overlap test 1", !overlaps_rect(a,b));
	assert("Rect overlap test 2", !overlaps_rect(b,a));
	assert("Rect overlap test 3", overlaps_rect(a,c));
	assert("Rect overlap test 4", overlaps_rect(a,d));
	assert("Rect overlap test 5", overlaps_rect(c,a));
	assert("Rect overlap test 6", overlaps_rect(b,c));
	assert("Rect overlap test 7", overlaps_rect(c,b));
	assert("Rect overlap test 8", overlaps_rect(d,a));
}

char *circle_overlaps_test() {
	Circ a = circ_new(0.f, 0.f, 5.f);
	Circ b = circ_new(10.f, 10.f, 2.f);
	Circ c = circ_new(5.f, 5.f, 32.f);
	assert("Circle overlap test 1", !overlaps_circ(a, b));
	assert("Circle overlap test 2", !overlaps_circ(b, a));
	assert("Circle overlap test 3", overlaps_circ(a, c));
	assert("Circle overlap test 4", overlaps_circ(c, a));
	assert("Circle overlap test 5", overlaps_circ(b, c));
	assert("Circle overlap test 6", overlaps_circ(c, b));
}

char *rect_circ_overlaps_test() {
	Rect a = rect_new(0.f, 0.f, 32.f, 32.f); 
	Circ b = circ_new(0.f, 0.f, 5.f);
	Rect c = rect_new(-10.f, -20.f, 1.f, 5.f);
	assert("Rect circle overlap test 1", overlaps_rect_circ(a, b));
	assert("Rect circle overlap test 2", !overlaps_rect_circ(c, b));
}

char *tests() {	
	run_test(vector_length);
	run_test(vector_scale);
	run_test(vector_arithmetic);
	run_test(vector_rotation);
	run_test(line_intersect);
	run_test(rect_intersect);
	run_test(rect_engulf_test);
	run_test(rect_overlaps_test);
	run_test(circle_overlaps_test);
	run_test(rect_circ_overlaps_test);
}

#undef main
int main() {
     char *result = tests();
     if (result != NULL) {
         printf("Failed:\t%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);
 
     return result != 0;
}
