#include <stdio.h>

#include "arcade.h"
#include "batch.h"
#include "window.h"

void update(World world, ArcadeObject *obj, void *data) {}

void collision(World world, ArcadeObject *a, void *adata, ArcadeObject *b, void *bdata) {}

int main() {
	Window window = window_new(window_config_new(640, 480, "Example")); // segfaults here
	World world = world_new(640, 480, 32, 0);
	Texture tex = texture_new("img.png");
	TextureRegion region = texregion_new(tex);
	Animation anim = anim_new(1);
	anim_add_frame(&anim, region);
	ArcadeObject obj = arcobj_new(shape_rect(rect_new(0, 0, 32, 32)), false, anim);
	world_add(&world, obj, NULL);
	while(window_should_contine(window)) {
		window_begin(&window);
		world_update(world, 10, update, collision);
		world_draw(world, window.batch);
		window_end(&window);
	}
	texture_destroy(tex);
	world_destroy(world);
	window_destroy(window);
}
