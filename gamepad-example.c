#include "arcade.h"

#include <stdio.h>

void update(World world) {}

void obj_update(World world, ArcadeObject *obj, void *data) {
	ArrayList controllers = world.window->controllers;
	for(size_t i = 0; i < controllers.length; i++) {
		GamepadState *gs = al_get(controllers, i);
		if(gs->left.x < 0) {
			obj->velocity.x = -3;
		} else if(gs->left.x > 0) {
			obj->velocity.x = 3;
		} else if(gs->left.y < 0) {
			obj->velocity.y = -3;
		} else if(gs->left.y > 0) {
			obj->velocity.y = 3;
		}
		printf("(%f:%f), (%f:%f)\n", gs->left.x, gs->left.y, gs->right.x, gs->right.y);
	}
	printf("%d\n", SDL_NumJoysticks());
	obj->sprite.position = shape_get_position(obj->bounds);
}

void collision(World world, ArcadeObject *a, void *adata, ArcadeObject *b, void *bdata) {}

int main() {
	Window window = window_new(window_config_new(640, 480, "Gamepad Example"));
	World world = world_new(&window, 640, 480, 32, 0);
	Texture tex = texture_new(window, "img.png");
	TextureRegion region = texregion_new(tex);
	Sprite spr = spr_new_static(region, vec2_new(0, 0));
	ArcadeObject obj = arcobj_new(shape_rect(rect_new(0, 0, 32, 32)), false, spr);
	world_add(&world, obj, NULL);
	while(window_should_contine(window)) {
		window_events(&window);
		world_update(world, update, obj_update, collision);
		window_start_draw(&window, 0, 0, 0);
		world_draw(world);
		window_end_draw(window);
	}
	texture_destroy(tex);
	world_destroy(world);
	window_destroy(window);
}
