#pragma once

#include "rect.h"

typedef struct {
	ArrayList vertex_buffer, element_buffer;
} DrawCall;

DrawCall dc_new();
void dc_add(DrawCall *call, Rect area, Transform transform);
void dc_draw(DrawCall call);
