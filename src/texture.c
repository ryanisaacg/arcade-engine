#include "texture.h"

#include <SOIL/SOIL.h>

Texture texture_new(char *texture_path) {
	GLuint texture[1];
	glGenTextures(1, texture);
	Texture tex = { 0, 0, texture[0] };
	glActiveTexture(GL_TEXTURE0 + texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	unsigned char *image = SOIL_load_image(	texture_path, &tex.width, &tex.height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}

void texture_bind(Texture tex, Program prog, char *name) {
	glUniform1i(glGetUniformLocation(prog.id, name), tex.id);
}

void texture_destroy(Texture tex) {
	glDeleteTextures(1, &tex.id);
}
