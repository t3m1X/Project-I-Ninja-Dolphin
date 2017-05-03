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
	graphics = App->textures->Load("revamp_spritesheets/particles_spritesheet.png");

	autoattack.anim.SetUp(0, 0, 5, 14, 4, 4, "0,1,2,3");
	autoattack.anim.loop = false;
	autoattack.anim.speed = 0.3f;
	autoattack.life = 1500;
	autoattack.speed = { 0, -14};
	
	
	explosion.anim.SetUp(0, 14, 69, 66, 8, 8, "0,1,2,3,4,5,6,7");
	explosion.anim.loop = false;
	explosion.anim.speed = 0.19f;
	explosion.life = 700;
	explosion.speed = { 0, 0};
	explosion.fx = App->audio->LoadSFX("sfx/destroy_b_air.wav");
	

	enemyshot.anim.SetUp(20, 0, 8, 8, 4, 4, "0,1,2,3");
	enemyshot.anim.loop = true;
	enemyshot.anim.speed = 0.3f;
	enemyshot.life = 1500;
	enemyshot.speed = { 0, -7};

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);
	autoattack.anim.CleanUp();
	explosion.anim.CleanUp();
	enemyshot.anim.CleanUp();
	
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
			iPoint view = { 0,1 };
			if (p->speed != iPoint(0, 0))
				view = p->speed;
			App->render->Blit(6, graphics, p->position.x, p->position.y, view, &(p->anim.GetCurrentFrame()));
			if (p->fx_played == false)
			{
				p->fx_played = true;
				// Play particle fx here
				if (p->fx != NULL)
				{
					App->audio->PlaySFX(explosion.fx);
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(particle_type type, int x, int y, fPoint direction, Uint32 delay)
{
	
	Particle* p = nullptr;
	switch (type) {
	case AUTOSHOT:
		p = new Particle(autoattack);
		p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		break;

	case EXPLOSION:
		p = new Particle(explosion);
		break;

	case ENEMYSHOT:
		p = new Particle(enemyshot);
		p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_ENEMY_SHOT, this);
		break;
		
	}
	if (direction.x != 999 && direction.y != 999) {
		direction.Normalize();
		direction = direction * p->speed.Length();
		p->speed.x = direction.x;
		p->speed.y = direction.y;
		//p->speed.x = direction.x;
		//p->speed.y = direction.y;
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

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	return ret;
}
