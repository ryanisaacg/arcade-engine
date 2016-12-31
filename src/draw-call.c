#include "draw-call.h"

DrawCall dc_new() {
	DrawCall call;
	call.vertex_buffer = al_new(sizeof(GLfloat));
	call.element_buffer = al_new(sizeof(GLuint));
	call.elements = 0;
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

void dc_add(DrawCall *call, Rect area, Transform transform) {

}

void dc_draw(DrawCall call) {

}

