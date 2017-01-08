#include <stdio.h>

#include "arcade.h"
#include "util.h"

void update(World world, ArcadeObject *obj, void *data) {}

void collision(World world, ArcadeObject *a, void *adata, ArcadeObject *b, void *bdata) {}

int main() {
	Window window = window_new(window_config_new(640, 480, "Example")); // segfaults here
	window.batch = batch_new();
	Batch *batch = &(window.batch);
	World world = world_new(batch, 640, 480, 32, 0);
	Texture tex = texture_new("img.png");
	TextureRegion region = texregion_new(tex);
	Sprite spr = spr_new_static(region, rect_new(0, 0, 32, 32));
	ArcadeObject obj = arcobj_new(shape_rect(rect_new(0, 0, 32, 32)), false, spr, batch);
	world_add(&world, obj, NULL);
	while(window_should_contine(window)) {
		window_update(window);
		world_update(world, 10, update, collision);
		window_draw(window);
	}
	texture_destroy(tex);
	world_destroy(world);
	window_destroy(window);
}
