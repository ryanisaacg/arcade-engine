#include "shaders.h"

#include "util.h"

Shader shader_vertex_new(const GLchar const *data) {
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &data, NULL);
	glCompileShader(shader);
	print_gl_error("Shader compilation");
	return (Shader) { shader };
}

Shader shader_fragment_new(const GLchar const *data) {
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &data, NULL);
	glCompileShader(shader);
	print_gl_error("Shader compilation");
	return (Shader) { shader };
}

void shader_destroy(Shader shader) {
	glDeleteShader(shader.id);
}

Program program_new(Shader vertex, Shader fragment, char *out_color_name) {
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex.id);
	print_gl_error("Attach vertex shader");
	glAttachShader(shader_program, fragment.id);
	print_gl_error("Attach fragment shader");
	glBindFragDataLocation(shader_program, 0, out_color_name);
	print_gl_error("Shader program output");
	glLinkProgram(shader_program);
	print_gl_error("Link shader program");
	glUseProgram(shader_program);
	print_gl_error("Use shader program");
	Program program = { shader_program, vertex, fragment };
	return program;
}

void program_add_attribute(	Program shader, const GLchar const *name, int members, 
							int distance_between, int start_offset) {
	GLint attrib = glGetAttribLocation(shader.id, name);
	print_gl_error("Get attribute location");
	glEnableVertexAttribArray(attrib);
	print_gl_error("GL Enable vertex attribute array");
	glVertexAttribPointer(attrib, members, GL_FLOAT, GL_FALSE, 
								distance_between * sizeof(GLfloat),
								(void*)(start_offset * sizeof(GLfloat)));
	print_gl_error("GL vertex attrib pointer");
}

void program_destroy(Program program) {
	glDeleteProgram(program.id);
}

