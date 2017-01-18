#include <stdio.h>

#include "arcade.h"

void update(World world, ArcadeObject *obj, void *data) {
	obj->sprite.position = shape_get_position(obj->bounds);
}

void collision(World world, ArcadeObject *a, void *adata, ArcadeObject *b, void *bdata) {}

int main() {
	Window window = window_new(window_config_new(640, 480, "Example")); // segfaults here
	World world = world_new(&window, 640, 480, 32, 0);
	Texture tex = texture_new(window, "img.png");
	TextureRegion region = texregion_new(tex);
	Sprite spr = spr_new_static(region, vec2_new(0, 0));
	ArcadeObject obj = arcobj_new(shape_rect(rect_new(0, 0, 32, 32)), false, spr);
<<<<<<< HEAD
	obj.acceleration.y = 0.01;
	world_add(&world, obj, NULL);
	while(window_should_contine(window)) {
		window_events(&window);
		//world_update(world, 1, update, collision);
		spr.bounds.y += 1;
		window_start_draw(window, 0, 0, 0);
		window_draw(window, spr);
		//world_draw(world);
=======
	obj.velocity.y = 1;
	world_add(&world, obj, NULL);
	while(window_should_contine(window)) {
		window_events(&window);
		world_update(world, 1, update, collision);
		window_start_draw(&window, 0, 0, 0);
		world_draw(world);
>>>>>>> aeea13028dbfca0939eb5a55800286e5f5c283d8
		window_end_draw(window);
	}
	texture_destroy(tex);
	world_destroy(world);
	window_destroy(window);
}
