#include "texture.h"

Texture texture_new(Window window, char *texture_path) {
}

void texture_destroy(Texture tex) {
	SDL_DestroyTexture(tex.texture);
}
