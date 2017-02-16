#pragma once

#include "geom.h"
#include "graphics.h"

///Defines the various behaviors a particle can have when contacting a physics object
typedef enum { DIES_ON_CONTACT, BOUNCE_ON_CONTACT, NO_CONTACT_ACTION } ParticleContactAction;

/**
 * \brief A single particle for particle effects
 *
 * Particles are dynamic objects that cannot be modified during their lifetime, die after a given number of ticks, and do not interact with physics objects
 * by default. If they do, the interaction is one-way: e.g. particles may bounce or die on contact, but may not influence game state in any way.
 */
typedef struct Particle {
	///\private
	Vector2 velocity;
	///\private
	Vector2 acceleration;
	///\private
	Vector2 jerk;
	///\private
	Sprite sprite;
	///\private
	float rotational_velocity;
	///\private
	int life;
} Particle;

/**
 * \brief A structure that defines the possible parameters of Particle objects
 *
 * Constructed with sane defaults, but for best results configure all relevant options
 *
 * Velocity- The change in position each frame
 *
 * Acceleration- The change in velocity each frame
 *
 * Jerk- The change in acceleration each frame
 *
 * Acceleration angle is relative to velocity, and jerk is relative to acceleration
 */
typedef struct ParticleEmitter {
	///A default copy of the sprite to stamp on Particle objects that are emitted
	Sprite sprite;
	///The smallest angle in degrees the particle can spawn moving at
	float min_direction;
	///The largest angle in degrees the particle can spawn moving at
	float max_direction;
	///The smallest magnitude of velocity the particle can spawn with
	float min_speed;
	///The largest magnitude of velocity the particle can spawn with
	float max_speed;
	///The smallest angle of acceleration the particle can spawn with
	float min_accel_dir;
	///The largest angle of acceleration the particle can spawn with
	float min_accel_dir;
	///The smallest acceleration magnitude the particle can spawn with
	float min_accel;
	///The largest acceleration magnitude the particle can spawn with
	float max_accel;
	///The smallest angle in degrees of jerk the particle can spawn with
	float min_jerk_dir;
	///The largest angle in degrees of jerk the particle can spawn with
	float max_jerk_dir;
	///The smallest magnitude of jerk the particle can spawn with
	float min_jerk;
	///The maximum magnitude of jerk the particle can spawn with
	float max_jerk;
	///The minimum angle of the sprite the particle can spawn with
	float min_rotation;
	///The maximum angle of the sprite the particle can spawn with
	float max_rotation;
	///The minimum amount the sprite will spin per frame
	float min_rotational_velocity
	///The maximum amount the sprite will spin per frame
	float max_rotational_velocity;
	///The minimum amount of frames the sprite will live
	int min_life;
	///The maximum amount of frames the sprite will live
	int max_life;
} ParticleEmitter;

///Apply transformations to a particle
void part_step(Particle *part);

///Create a particle emitter with the given parameters
ParticleEmitter pe_new(const Sprite sprite, const int min_life, const int max_life);
///Spawn a new particle using the emitter
Particle pe_spawn(const ParticleEmitter *pe);
///Add a particle to the given array list
void pe_add_to(const ParticleEmitter *pe, ArrayList *particles);
