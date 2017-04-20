#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleCollision.h"

#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;

enum particle_type {
	AUTOSHOT = 0,
	EXPLOSION,
};

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	particle_type type;
	bool fx_played = false;

	Particle();
	Particle(const Particle& p);
	bool Update();
};
class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status Update();
	bool CleanUp();

	
	void AddParticle(particle_type type, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE, Uint32 delay = 0);

private:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphics_explosion = nullptr;
	Particle* active[MAX_ACTIVE_PARTICLES];
	uint last_particle = 0;

public:

	Particle autoattack;
	Particle explosion;
	
};

#endif // __MODULEPARTICLES_H__
