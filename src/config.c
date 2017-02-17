#include "config.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>

static bool str_equal(void *a, void *b) {
	return strcmp(a, b) == 0;
}

Document config_new(char *contents) {
	Document doc = {
		.items = hm_new_eqfunc(str_equal)
	};
	char *current_section = NULL;
	while(*contents) {
		if(*contents == ':') {
			char *section_start = contents + 1;
			while(*(++contents) != '\n'); //consume all characters not including the newline
			size_t length = contents - section_start;
			current_section = malloc(length + 1);
			memcpy(current_section, section_start, length);	
			current_section[length] = '\0';
			hm_put(doc.items, *current_section, current_section, hm_new_eqfunc(str_equal)); //Put a new map into the map
		} else {
			char *key_start = contents;
			while(*(++contents) != '='); //consume all characters up to but not including the equal
			size_t length = contents - key_start;
			char *key = malloc(length + 1);
			memcpy(key, key_start, length);
			key[length] = '\0';
			char *value_start = contents + 1;
			float *data = malloc(sizeof(float) * 3);
			float *current_data = data;
			char value[1024];
			while(*contents != '\n') { //while more values remain
				contents++;
				if(*contents == ',' || *contents == '\n') {
					length = contents - value_start;
					memcpy(value, value_start, length);
					value[length] = '\0';
					*current_data = atof(value);
					value_start = contents + 1;
					current_data++;
				}
			}	
			HashMap *pairs = hm_get(doc.items, *current_section, current_section); //Get the key-value pairs of the current section
			hm_put(pairs, *key, key, data); //put this key-value pair into it
		}
		contents++;
	}
	return doc;
}

void config_concat(Document source, Document dest) {
	ArrayList keys = hm_get_keys(source.items);
	for(size_t i = 0; i < keys.length; i++) {
		char *key = al_get(keys, i);
		hm_put(dest.items, *key, key, hm_get(source.items, *key, key));
	}
}

ArrayList config_get_sections(Document doc) {
	return hm_get_keys(doc.items);
}

ArrayList config_get_keys(Document doc, char *section) {
	return hm_get_keys(hm_get(doc.items, *section, section));
}

static float zero[3] = { 0 };

float *config_get_value(Document doc, char *section_name, char *key) {
	HashMap *section = hm_get(doc.items, *section_name, section_name);
	if(section == NULL) {
		return zero;
	} else {
		float *value = hm_get(section, *key, key);
		if(value == NULL) {
			return zero;
		} else {
			return value;
		}
	}
	return hm_get(section, *key, key);
}
