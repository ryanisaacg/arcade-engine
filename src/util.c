#include "util.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool equal_float(float a, float b) {
	return fabs(a - b) < 0.00001f;
}


/*
Reads a file in its entirety
Intended to be immune to buffer overflows
Will hang forever if given stdin, for example, so don't do that
*/
char* read_file(char *filename) {
	FILE *in = fopen(filename, "r");
	char chunk[1024];
	char *data = malloc(2048);
	data[0] = '\0';
	chunk[0] = '\0';
	size_t data_length = 0, data_capacity = 2048;
	while(!feof(in)) {
		size_t len = fread(chunk, 1, 1023, in);
		chunk[len] = '\0';
		if(ferror(in)) {
			fprintf(stderr, "File operation failed in read_file\n");
		}
		if(data_length + len < data_capacity / 2) {
			strcat(data, chunk);
		} else {
			data_capacity *= 2;
			printf("%s\n", data);
			data = realloc(data, data_capacity);
			if(data == NULL) {
				fprintf(stderr, "Realloc failed in read_file\n");
			}
			strcat(data, chunk);
		}
		data_length += len;
	}
	fclose(in);
	return data;
}

Rect sdl_rect_conv(SDL_Rect rect) {
	return (Rect) { rect.x, rect.y, rect.w, rect.h };
}

SDL_Rect rect_conv(Rect rect) {
	return (SDL_Rect){ (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height };
}

float random_float(float min, float max) {
	float generated = rand();
	return generated / RAND_MAX * (max - min) + min; 
}

bool str_equal(void *a, void *b) {
	return strcmp(a, b) == 0;
}
