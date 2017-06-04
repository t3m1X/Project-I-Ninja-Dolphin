#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"

#define MAX_ACTIVE_PARTICLES 200

struct SDL_Texture;

enum particle_type {
	AUTOSHOT = 0,
	LASERSHOT,
	LASERBIGSHOT,
	BIGASSLASER,
	MISSILE,
	EXPLOSION,
	ENEMYSHOT,
	CRATER,
	BIG_EXPLOSION,
	LIGHT_EXPLOSION,
	TURRET_CRATER,
	BOMBSHOT,
	BOMB_EXPLOSION,
	PLAYER_EXPLOSION,
	PLAYER_BITS
};

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	Mix_Chunk* fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	particle_type type;
	bool fx_played = false;
	bool to_delete = false;
	int layer;

	Particle();
	Particle(const Particle& p);
	virtual bool Update();
};

struct ACParticle : public Particle
{
private:
	uint sdl_acc = 0;
	int iterations = 0;

public:
	iPoint acceleration; //Every 100ms

	ACParticle() : Particle()
	{}
	ACParticle(const Particle& p) : Particle(p)
	{}
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

	
	void AddParticle(particle_type type, int x, int y, fPoint direction = { 999,999 }, bool player1 = true, Uint32 delay = 0);

	void OnCollision(Collider * c1, Collider * c2);

private:

	SDL_Texture* graphics = nullptr;
	Particle* active[MAX_ACTIVE_PARTICLES];
	uint last_particle = 0;

public:

	Particle autoattack;
	Particle laserattack;
	Particle laserattbig;
	Particle bigasslaser;
	ACParticle missile;
	Particle explosion;
	Particle enemyshot;
	Particle crater;
	Particle big_explosion;
	Particle light_explosion;
	Particle turret_crater;
	Particle bombshot;
	Particle bombexplosion;
	Particle player1_explosion;
	Particle player2_explosion;
	Particle player1_pieces;
	Particle player2_pieces;

	Mix_Chunk* destroy_b_air;
};

#endif // __MODULEPARTICLES_H__
