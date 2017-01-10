#include "texture.h"

#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

Texture texture_new(Window window, char *texture_path) {
	SDL_Texture *tex = NULL;
	SDL_Surface *loaded = IMG_Load(texture_path);
	if(loaded == NULL) {
		goto err;
	} else {
		tex = SDL_CreateTextureFromSurface(window.rend, loaded);
		if(tex == NULL) {
			goto err;
		}
		SDL_FreeSurface(loaded);
	}
	Texture texture;
	texture.texture = tex;
	SDL_QueryTexture(tex, NULL, NULL, &(texture.width), &(texture.height));
	return texture;
	err:
	fprintf(stderr, "Unable to load image %s. \n\tSDL Error: %s\n\tSDL_Image Error: %s", texture_path, SDL_GetError(), IMG_GetError());
	exit(-1);
}

void texture_destroy(Texture tex) {
	SDL_DestroyTexture(tex.texture);
}
