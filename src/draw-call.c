#include "draw-call.h"

DrawCall dc_new() {
	DrawCall call;
	call.vertex_buffer = al_new(sizeof(GLfloat));
	call.element_buffer = al_new(sizeof(GLuint));
	call.elements = 0;
	glGenBuffers(1, &call.vbo_id);
	glGenBuffers(1, &call.ebo_id);
	return call;
}

void dc_clear(DrawCall *call) {
	al_clear(&call->vertex_buffer);
	al_clear(&call->element_buffer);
	call->elements = 0;
}

void dc_add_manual(DrawCall *call, GLfloat *vertices, size_t vertex_size, 
									GLuint *elements, size_t num_elements) {
	while(vertex_size > 0) {
		al_add(&call->vertex_buffer, vertices);
		vertices++;
		vertex_size--;
	}
	while(num_elements > 0) {
		al_add(&call->element_buffer, elements);
		elements++;
		num_elements--;
	}
}

void dc_add(DrawCall *call, Rect texture_source, Rect area, Transform transform) {
	GLfloat vertex_items[16];//2 floats for world position, 2 floats for texture position
							 //4 vertices
	GLuint elements[6] = { 0, 1, 2, 2, 3, 0 };
	size_t i = 0;
	for(int i = 0; i <= 1; i++) {
		for(int j = 0; j <= 1; j++) {
			Vector2 point = trans_apply(transform, 
					vec2_new(area.x + area.width * i, area.y + area.height * j));
			Vector2 texture_point = vec2_new(texture_source.x + texture_source.width * i,
					texture_source.y + texture_source.height * j);
			vertex_items[i++] = point.x;
			vertex_items[i++] = point.y;
			vertex_items[i++] = texture_point.x;
			vertex_items[i++] = texture_point.y;
		}
	}
	dc_add_manual(call, 
			vertex_items, sizeof(vertex_items) / sizeof(GLfloat),
			elements, sizeof(elements) / sizeof(GLuint));
}

void dc_draw(DrawCall call) {
	glBindBuffer(GL_ARRAY_BUFFER, call.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, call.vertex_buffer.length * sizeof(GLfloat),
			al_get(call.vertex_buffer, 0), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, call.ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, call.element_buffer.length * sizeof(GLuint),
			al_get(call.element_buffer, 0), GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, call.element_buffer.length, GL_UNSIGNED_INT, 0);
}

