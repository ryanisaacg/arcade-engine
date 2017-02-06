#include "config.h"
#include "util.h"

Document config_new(char *filepath) {
	char *contents = read_file(filepath);
}

ArrayList config_get_sections(Document doc);
ArrayList config_get_keys(Document doc, char *section);
float *config_get_value(Document doc, char *section, char *key);