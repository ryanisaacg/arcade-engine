#pragma once

#include <GL/GLEW.h>

typedef GLuint Shader;

Shader load_shader(char *vertex_filename, char *fragment_filename);
