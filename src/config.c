#include "config.h"
#include "util.h"

#include <string.h>

Document config_new(char *contents) {
	char line[1024];
	char *line_position = line;
	Document doc = {
		.items = hm_new()
	};
	char *current_section = NULL;
	while(*contents) {
		while(*contents != '\n') { //consume characters until a newline is found
			*line_position = *contents;
			contents++;
			line_position++;
		}
		*line_position = '\0'; //Null terminate the line string
		line_position = line;
		if(line[0] == '[') {
			//Parse a section header
			char *end = line;
			while(*end != ']') end++; //Move to the next closing bracket
			current_section = malloc(end - line); 
			memcpy(current_section, line, end - line); //Copy the section name
			hm_put(doc.items, *current_section, current_section, hm_new()); //Put a new map into the map
		} else {
			char *check = line;
			bool contents = false; //If any non-whitespace characters were found
			while(!contents && check - line < 1024) {  //check for non_whitespace characters
				contents = *check != ' ' || *check != '\t' || *check != '\r' || *check != '\n'; 
				check++;
			} 
			if(!contents) { //If the line was empty
				continue;
			}
			char *end_key = line;
			while(*end_key != '=') end_key++; //Seek the end of the key
			char *key = malloc(end_key - line);
			memcpy(key, line, end_key - line);
			char *start_value = end_key;
			char *end_value = end_key;
			float *data_start = malloc(sizeof(float) * 3);
			float *data = data_start;
			while(*end_value != '\n') { //Seek the values
				end_value++;
				if(*end_value == ',' || *end_value == '\n') { //Seek the end of a value
					*data = strtof(start_value, &end_value);
					data++;
					start_value = end_value;
				}
			}
			HashMap *pairs = hm_get(doc.items, *current_section, current_section); //Get the key-value pairs of the current section
			hm_put(pairs, *key, key, data_start); //put this key-value pair into it
		}
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

float *config_get_value(Document doc, char *section, char *key) {
	return hm_get(hm_get(doc.items, *section, section), *key, key);
}
