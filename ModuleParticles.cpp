#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"

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
	enemyshot.speed = { 0, -5};

	crater.anim.SetUp(0, 157, 66, 60, 3, 3, "0,1,2");
	crater.anim.loop = true;
	crater.anim.speed = 0.2f;
	crater.life = 8000;
	crater.speed = { 0,0 };

	big_explosion.anim.SetUp(0, 216, 135, 126, 4, 8, "0,1,2,3,4,5,6,7");
	big_explosion.anim.loop = false;
	big_explosion.anim.speed = 0.2f;
	big_explosion.life = 700;
	big_explosion.speed = { 0,0 };
	big_explosion.fx = App->audio->LoadSFX("sfx/destroy_b_air.wav");

	laserattack.anim.SetUp(81, 0, 2, 16, 1, 1, "0");
	laserattack.anim.loop = true;
	laserattack.speed = { 0, -14 };
	laserattack.life = 1500;

	light_explosion.anim.SetUp(0, 874, 64, 60, 7, 12, "0,1,2,3,4,5,6,7,8,9,10,11");
	light_explosion.anim.loop = false;
	light_explosion.anim.speed = 0.2f;
	light_explosion.life = 600;
	light_explosion.fx = App->audio->LoadSFX("sfx/destroy_b_air.wav");

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);
	autoattack.anim.CleanUp();
	laserattack.anim.CleanUp();
	explosion.anim.CleanUp();
	enemyshot.anim.CleanUp();
	crater.anim.CleanUp();
	big_explosion.anim.CleanUp();
	light_explosion.anim.CleanUp();
	
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

		if (p->Update() == false || p->to_delete == true)
		{
			App->collision->EraseCollider(p->collider);
			p->collider = nullptr;
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			iPoint view = { 0,1 };
			if (p->speed != iPoint(0, 0))
				view = p->speed;
			App->render->Blit(p->layer, graphics, p->position.x, p->position.y, view, &(p->anim.GetCurrentFrame()));
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
		p->layer = 6;
		break;

	case LASERSHOT:
		p = new Particle(laserattack);
		p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		p->layer = 6;
		break;

	case EXPLOSION:
		App->input->ShakeController(1, 500, 0.1f);
		App->input->ShakeController(2, 500, 0.1f);
		p = new Particle(explosion);
		p->layer = 6;
		break;

	case ENEMYSHOT:
		p = new Particle(enemyshot);
		p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_ENEMY_SHOT, this);
		p->layer = 6;
		break;

	case CRATER:
		p = new Particle(crater);
		p->layer = 2;
		break;

	case BIG_EXPLOSION:
		App->input->ShakeController(1, 500, 0.3f);
		App->input->ShakeController(2, 500, 0.3f);
		p = new Particle(big_explosion);
		p->layer = 6;
		break;

	case LIGHT_EXPLOSION:
		App->input->ShakeController(1, 500, 0.1f);
		App->input->ShakeController(2, 500, 0.1f);
		p = new Particle(light_explosion);
		p->layer = 6;
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

	bool found = false;

	for (int i = 0; i < MAX_ACTIVE_PARTICLES; ++i) 		{
		if (active[i] == nullptr) 			{
			found = true;
			active[i] = p;
			break;
		}
	}

	if (!found) {
		LOG("Overwriting old particles");

		if (active[last_particle] != nullptr) {
			Particle* temp = active[last_particle];
			App->collision->EraseCollider(temp->collider);
			temp->collider = nullptr;
			delete temp;
			active[last_particle] = nullptr;
		}

		active[last_particle++] = p;
		if (last_particle >= MAX_ACTIVE_PARTICLES)
			last_particle = 0;		
	}
}

// -------------------------------------------------------------
void ModuleParticles::OnCollision(Collider* c1, Collider* c2) {
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
		if (active[i] != nullptr && active[i]->collider == c1) {
			active[i]->to_delete = true;
			break;
		}
	}
}
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
		if (anim.Finished()) {
			ret = false;
			to_delete = true;
		}

	position.x += speed.x;
	position.y += speed.y;

	App->collision->SetPosition(collider, position.x, position.y);

	return ret;
}
