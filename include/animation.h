#pragma once

#include "array_list.h"
#include "texture-region.h"

typedef struct {
	ArrayList frames;
	int current_time, time_per_frame, current_frame;
} Animation;

Animation anim_new();
void anim_add_frame(Animation anim, TextureRegion texture);
