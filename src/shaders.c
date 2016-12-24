#include "shaders.h"

#include "util.h"

Shader load_shader(char *vertex_filename, char *fragment_filename) {
	GLchar const *vertex = (GLchar*)read_file(vertex_filename);
	GLchar const *fragment = (GLchar*)read_file(fragment_filename);
	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex, NULL);
	glCompileShader(vertexShader);
	print_gl_error("Vertex shader");
	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment, NULL);
	glCompileShader(fragmentShader);
	print_gl_error("Fragment shader");

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	print_gl_error("Attach vertex shader");
	glAttachShader(shaderProgram, fragmentShader);
	print_gl_error("Attach fragment shader");
	glBindFragDataLocation(shaderProgram, 0, "out_color");
	print_gl_error("Shader program output");
	glLinkProgram(shaderProgram);
	print_gl_error("Link shader program");
	glUseProgram(shaderProgram);
	print_gl_error("Use shader program");
	return shaderProgram;
}
