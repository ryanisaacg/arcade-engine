#pragma once

#include "rect.h"
#include "texture.h"
#include "transform.h"

struct TextureRegion {
	Texture source;
	Rect region;
};

TextureRegion texregion_new(Texture source);
TextureRegion texregion_new_sized(Texture source, Rect region);
TextureRegion texregion_slice(TextureRegion region, Rect subregion);

