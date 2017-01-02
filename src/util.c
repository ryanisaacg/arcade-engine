#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

void print_gl_error(char *label) {
	GLenum glenum = glGetError();
	switch(glenum) {
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		printf("%s:%s", label, "GL Error: Invalid enum\n");
		break;
	case GL_INVALID_VALUE:
		printf("%s:%s", label, "GL Error: Invalid value\n");
		break;
	case GL_INVALID_OPERATION:
		printf("%s:%s", label, "GL Error: Invalid operation\n");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		printf("%s:%s", label, "GL Error: Invalid framebuffer operation");
		break;
	case GL_OUT_OF_MEMORY:
		printf("%s:%s", label, "GL Error: Out of memory");
		break;
	}
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
