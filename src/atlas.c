#include "atlas.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/GLEW.h>
#include <SOIL/SOIL.h>

TextureAtlas atlas_new(char *filename, Shader shaderProgram) {
	GLuint textures[1];
	glGenTextures(1, textures);
	int width, height;
	glActiveTexture(GL_TEXTURE0);
	unsigned char *image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
   	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "image"), 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return (TextureAtlas) { width, height };
}
