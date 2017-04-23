#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"


#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("revamp_spritesheets/bullets_spritesheet.png");
	graphics_explosion = App->textures->Load("revamp_spritesheets/explosion1_spritesheet.png");

	autoattack.anim.SetUp(0, 0, 57, 49, 4, 4, "0,1,2,3");

	//autoattack.anim.PushBack({ 2, 9, 2, 6 });
	//autoattack.anim.PushBack({ 6, 9, 2, 6 });
	//autoattack.anim.animation = new int[2];
	//autoattack.anim.animation[0] = 0;
	//autoattack.anim.animation[1] = 1;
	autoattack.anim.loop = true;
	autoattack.anim.speed = 0.3f;
	autoattack.life = 1500;
	autoattack.speed = { 0, -16};
	
	
	explosion.anim.PushBack({ 142, 0, 52, 52 });
	explosion.anim.PushBack({ 208, 0, 63, 64 });
	explosion.anim.PushBack({ 281, 0, 61, 64 });
	explosion.anim.PushBack({ 348, 0, 55, 64 });
	explosion.anim.PushBack({ 414, 0, 65, 64 });
	explosion.anim.PushBack({ 481, 0, 69, 64 });
	explosion.anim.PushBack({ 550, 0, 69, 64 });
	explosion.anim.animation = new int[7];
	explosion.anim.animation[0] = 0;
	explosion.anim.animation[1] = 1;
	explosion.anim.animation[2] = 2;
	explosion.anim.animation[3] = 3;
	explosion.anim.animation[4] = 4;
	explosion.anim.animation[5] = 5;
	explosion.anim.animation[6] = 6;
	explosion.anim.loop = false;
	explosion.anim.speed = 0.3f;
	explosion.life = 10000;
	//explosion.speed = { 0, -8};
	

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);
	delete[] explosion.anim.animation;
	delete[] autoattack.anim.animation;
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			App->collision->EraseCollider(p->collider);
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
			if (p->fx_played == false)
			{
				p->fx_played = true;
				// Play particle fx here
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(particle_type type, int x, int y, Uint32 delay)
{
	
	Particle* p = nullptr;
	switch (type) {
	case AUTOSHOT:
		p = new Particle(autoattack);
		p->collider = App->collision->AddCollider(p->anim.frames[0], COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		break;

	case EXPLOSION:
		p = new Particle(explosion);
		break;
		
	}
	p->type = type;
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;
	active[last_particle++] = p;
	if (last_particle > MAX_ACTIVE_PARTICLES) {
		last_particle = 0;
		LOG("Overwriting old particles");
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

bool Particle::Update()
{
	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;

	position.x += speed.x;
	position.y += speed.y;

	collider->SetPos(position.x, position.y);

	return ret;
}
