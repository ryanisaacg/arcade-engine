#include <stdio.h>
#include "framework.h"

#include "geom.c"
#include "config.c"

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
	run_test(test_config_scalar);
	run_test(test_config_2d);
	return NULL;
}

int tests_run;

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
