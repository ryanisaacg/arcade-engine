#include "particles.h"

#include <stdlib.h>
#include <time.h>

#include "util.h"

void part_step(Particle *part) {
	part->sprite.position = vec2_add(part->sprite.position, part->velocity);
	part->velocity = vec2_add(part->velocity, part->acceleration);
	part->acceleration = vec2_add(part->acceleration, part->jerk);
	part->sprite.angle += part->rotational_velocity;
}

ParticleEmitter pe_new(const Sprite sprite, const int min_life, const int max_life) {
	srand(time(NULL));
	return (ParticleEmitter) {
		.sprite 					= sprite, 
		.min_life 					= min_life,
		.max_life					= max_life,
		.min_direction 				= 0,
		.max_direction				= 360, 
		.min_speed 					= 1,
		.max_speed					= 1,
		.min_accel_dir				= 0,
		.max_accel_dir				= 360,
		.min_accel					= 0,
		.max_accel					= 0,
		.min_jerk_dir				= 0,
		.max_jerk_dir				= 360,
		.min_rotation				= 0,
		.max_rotation 				= 360,
		.min_rotational_velocity 	= 0,
		.max_rotational_velocity 	= 360,
		.action = NO_CONTACT_ACTION
	};
}

Particle pe_spawn(const ParticleEmitter *pe, Vector2 location) {
	float rotation 			= random_float(pe->min_jerk_dir, pe->max_jerk_dir);
	float rotation_velocity = random_float(pe->min_rotational_velocity, pe->max_rotational_velocity);
	float direction			= random_float(pe->min_direction, pe->max_direction);
	float accel_direction	= random_float(pe->min_accel_dir, pe->max_accel_dir) + direction * pe->relative;
	float jerk_direction	= random_float(pe->min_jerk_dir, pe->max_jerk_dir) + accel_direction * pe->relative;
	Sprite sprite = pe->sprite;
	sprite.angle = rotation;
	sprite.position = location;
	return (Particle) {
		.velocity 				= vec2_new_directed(random_float(pe->min_speed, pe->max_speed), direction),
		.acceleration 			= vec2_new_directed(random_float(pe->min_accel, pe->max_accel), accel_direction),
		.jerk					= vec2_new_directed(random_float(pe->min_jerk, pe->max_jerk), jerk_direction),
		.rotational_velocity 	= rotation_velocity,
		.sprite 				= sprite,
		.action					= pe->action
	};
}

void pe_burst(const ParticleEmitter *pe, ArrayList *list, const Vector2 location, const int min, const int max) {
	int amount = rand() * (max - min) + min;
	for(int i = 0; i < amount; i++) {
		pe_add_to_list(pe, location, list);
	}
}

void pe_add_to_list(const ParticleEmitter *pe, Vector2 location, ArrayList *particles) {
	Particle particle = pe_spawn(pe, location);
	al_add(particles, &particle);
}
