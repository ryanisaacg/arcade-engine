#include "arcade.h"
#include "framework.h"

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
