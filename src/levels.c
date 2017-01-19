#include "levels.h"

#include <stdio.h>
#include <tmx.h>

//The side of a quad tree bucket square at the smallest level
#define QT_BUCKETS_SIZE 64

static void load_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current);
static void load_object_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current);
static void load_image_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current);

Level level_load(char *filename, AssetManager assets, size_t data_size) {
	tmx_map *map = tmx_load(filename);
	if(map == NULL) {
		tmx_perror("Loading the TileMap");
		exit(1);
	}
	World world = world_new(NULL, map->width, map->height, 64, data_size);
	//Retrieve the colors packed into the map->backgroundcolor
	world.r = map->backgroundcolor & 0xff;
	world.g = (map->backgroundcolor >> 8) & 0xff;
	world.b = (map->backgroundcolor >> 16) & 0xff;
	//Iterate over the linked list of map layers
	tmx_layer *current_layer = map->ly_head;
	while(current_layer != NULL) {
		switch(current_layer->type) {
		case L_NONE:
			break;
		case L_LAYER:
			load_layer(assets, &world, map, current_layer);
			break;
		case L_OBJGR:
			load_object_layer(assets, &world, map, current_layer);	
			break;
		case L_IMAGE:
			load_image_layer(assets, &world, map, current_layer);
			break;
		}
	}
}

static void load_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current) {

}

static void load_object_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current) {
	tmx_object *obj = current->content.objgr->head;
	while(obj != NULL) {
		float x = obj->x + current->offsetx;
		float y = obj->y + current->offsety;
		Shape shape;
		switch(obj->shape) {
		case S_NONE:
			break;
		case S_SQUARE:
			shape = shape_rect(rect_new(x, y, obj->width, obj->height));
			break;
		case S_POLYLINE:
			fprintf(stderr, "Attempted to load a Polyline object from a TMX file. Polyline objects are not supported (obj name %s)", obj->name);
			break;
		case S_POLYGON:
			//TODO: Add support for loading polygonal objects from Tiled
			break;
		case S_ELLIPSE:
			if(obj->width != obj->height) {
				fprintf(stderr, "Attempted to load a non-circular ellipse from a TMX file. Only circular ellipses are perimtted (obj name %s)", obj->name);
			}
			shape = shape_circ(circ_new(x, y, obj->width / 2));
			break;
		}
		if(obj->rotation != 0) {
			shape_set_rotation(&shape, obj->rotation);
		}
		tmx_tile *tile = tmx_get_tile(map, obj->gid);
		Sprite sprite;
		if(tile->animation) {
			TextureRegion *frames = malloc(sizeof(TextureRegion) * tile->animation_len);
		} else {
			Texture tex = asset_load(assets, tile->image->source);
			TextureRegion region = texregion_new_sized(tex, rect_new(tile->ul_x, tile->ul_y, tile->image->width, tile->image->height));
			sprite = spr_new_static(region, shape_get_position(shape));
		}
		//TODO: Loading of custom properties with function pointer
		world_add(world, arcobj_new(shape, false, sprite), NULL); //TODO: load if the object shoud be solid from the TMX file 
		obj = obj->next;
	}
}

static void load_image_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current) {

}

World level_get_world(Level level) {

}

void level_destroy(Level level) {
	world_destroy(level.data);
}
