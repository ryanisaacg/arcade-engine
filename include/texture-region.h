#pragma once

#include "rect.h"
#include "texture.h"

typedef struct {
	Texture source;
	Rect region;
	Transform transform;
} TextureRegion;

