#include "texture-region.h"

TextureRegion texregion_new(Texture source) {
	return (TextureRegion) { source, rect_new(0, 0, source.width, source.height), 
		trans_new_identity() };
}

TextureRegion texregion_new_sized(Texture source, Rect region) {
	return (TextureRegion) { source, region, trans_new_identity() };
}

TextureRegion texregion_slice(TextureRegion region, Rect subregion) {
	Rect sub = rect_new(region.region.x + subregion.y, region.region.y + subregion.x, 
			subregion.width, subregion.height);
	return (TextureRegion) { region.source, sub, trans_new_identity() };
}
