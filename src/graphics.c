#include "graphics.h"

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include "util.h"

const GLchar const *vertex_source = 
"#version 150 core\n"
"in vec2 position;"
"in vec2 tex_coord;"
"out vec2 Tex_coord;"
"void main() {"
"Tex_coord = tex_coord;"
"gl_Position = vec4(position, 0.0, 1.0); }";

const GLchar const *fragment_source =
"#version 150 core\n"
"in vec2 Tex_coord;"
"out vec4 out_color;"
"uniform sampler2D atlas;"
"void main() { out_color = texture(atlas, Tex_coord); }";


static GLuint compile_program(	const GLchar const *vertex_source, 
								const GLchar const *fragment_source) {
	GLuint vertex = compile_shader(vertex_source, GL_VERTEX_SHADER);
	GLuint fragment = compile_shader(fragment_source, GL_FRAGMENT_SHADER);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex);
	print_gl_error("Attach vertex shader");
	glAttachShader(shaderProgram, fragment);
	print_gl_error("Attach fragment shader");
	glBindFragDataLocation(shaderProgram, 0, "out_color");
	print_gl_error("Shader program output");
	glLinkProgram(shaderProgram);
	print_gl_error("Link shader program");
	glUseProgram(shaderProgram);
	print_gl_error("Use shader program");
	return shaderProgram;
}

static void setup_arrays(GLint shader_program) {
    GLint posAttrib = glGetAttribLocation(shader_program, "position");
	print_gl_error("Position attribute");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	print_gl_error("Position attribute pointer");

    GLint texAttrib = glGetAttribLocation(shader_program, "tex_coord");
	print_gl_error("Texture coordinate attribute");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	print_gl_error("Texture coordinate pointer");
}

void initialize(char *atlas_path, int *out_atlas_width, int *out_atlas_height) {
	GLuint shader = compile_program(vertex_source, fragment_source);
	GLuint textures[1];
	glGenTextures(1, textures);
	int width, height;
    
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    
	unsigned char *image = SOIL_load_image(atlas_path, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    
	glUniform1i(glGetUniformLocation(shader, "atlas"), 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	*out_atlas_width = width;
	*out_atlas_height = height;

	setup_arrays(shader);
}
