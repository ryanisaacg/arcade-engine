#include "arcade.h"
#include "framework.h"

#include "util.h"

char *test_config_scalar() {
	Document doc = config_new(
":Test\n"
"a=0\n"
"b=1\n"
"c=1.5\n");
	assert("Config get key", *config_get_value(doc, "Test", "a") == 0);
	assert("Config get key 2", *config_get_value(doc, "Test", "b") == 1);
	assert("Config get key 3", *config_get_value(doc, "Test", "c") == 1.5);
	return NULL;
}

char *test_config_2d() {
	Document doc = config_new(
":Test\n"
"a=0,5\n"
"b=0.1,0.5\n"
"c=6.11,7.12\n");
	float *val = config_get_value(doc, "Test", "a");
	assert("Config get key 2d", val[0] == 0 && val[1] == 5);
	val = config_get_value(doc, "Test", "b");
	assert("Config get key 2d 1", equal_float(val[0], 0.1) && equal_float(val[1], 0.5));
	val = config_get_value(doc, "Test", "c");
	assert("Config get key 2d 2", equal_float(val[0], 6.11) && equal_float(val[1], 7.12));
	return NULL;
}

char *test_config_3d() {
	Document doc = config_new(
":Test\n"
"a=0,5,6\n"
"b=0.1,0.5,1.2\n"
"c=6.11,7.12,3.55\n");
	float *val = config_get_value(doc, "Test", "a");
	assert("Config get key 3d", val[0] == 0 && val[1] == 5 && val[2] == 6);
	val = config_get_value(doc, "Test", "b");
	assert("Config get key 3d 1", equal_float(val[0], 0.1) && equal_float(val[1], 0.5) && equal_float(val[2], 1.2));
	val = config_get_value(doc, "Test", "c");
	assert("Config get key 3d 2", equal_float(val[0], 6.11) && equal_float(val[1], 7.12) && equal_float(val[2], 3.55));
	return NULL;
}
