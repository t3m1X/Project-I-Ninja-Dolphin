#include "Application.h"
#include "Enemy_MoonAirship.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"


Enemy_MoonAirship::Enemy_MoonAirship(int x, int y) : Enemy(x, y)
{
	fly.SetUp(629, 376, 98, 89, 4, 4, "0,1,2,3");
	fly.speed = 0.2f;

	shadow.SetUp(629, 555, 48, 32, 1, 1, "0");
	

	animation_shooting.SetUp(629, 465, 98, 89, 3, 3, "0,1,2");
	animation_shooting.speed = 0.2f;
	animation_shooting.loop = false;

	animation_hurt.SetUp(629, 465, 98, 89, 4, 4, "2,3,2,3,2");

	
	if (position.x > SCREEN_WIDTH / 2)
	{
		path.PushBack({ 0, 0.5f }, 120, &fly);
		path.PushBack({ 0,0 }, 30, &fly);
		path.PushBack({ -1,1 }, 50, &fly);
		path.PushBack({ -1,-2 }, 90, &fly);
		path.PushBack({ 0,-0.5 }, 40, &fly);
		path.PushBack({ 1,1 }, 50, &fly);
		path.PushBack({ 1,-2 }, 90, &fly);
		path.PushBack({ 0,-0.5 }, 40, &fly);
		path.LoopStart(150);
	}
	else 
	{
		path.PushBack({ 0,0.5f }, 120, &fly);
		path.PushBack({ 0,0 }, 30, &fly);
		path.PushBack({ 1,1 }, 50, &fly);
		path.PushBack({ 1,-2 }, 90, &fly);
		path.PushBack({ 0,-0.5 }, 40, &fly);
		path.PushBack({ -1,1 }, 50, &fly);
		path.PushBack({ -1,-2 }, 90, &fly);
		path.PushBack({ 0,-0.5 }, 40, &fly);
		path.LoopStart(150);
	}

	collider_offset.x = 8;
	collider_offset.y = 50;

	collider = App->collision->AddCollider({ 200, 0, 80, 30 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;
	

	sdl_clock_start = SDL_GetTicks() + 2500;

	type = AIRBORNE;
	hitpoints = 8;
}

Enemy_MoonAirship::~Enemy_MoonAirship()
{
	fly.CleanUp();
}

void Enemy_MoonAirship::Move()
{

	position = original_position + path.GetCurrentPosition(&animation);

	sdl_clock = SDL_GetTicks();

	if (sdl_clock >= sdl_clock_start &&  position.y - App->render->camera.y <= SCREEN_HEIGHT * 5 / 8) {
		shots++;
		iPoint origin = position;
		origin.x += 30;
		origin.y += fly.CurrentFrame().h - 16;
		Shoot(origin);
		origin.x += 30;
		Shoot(origin);

		if (shots >= 1) {
			sdl_clock_start = sdl_clock + 500;
			/*shots = 0;*/
		}
	}
	

}

void Enemy_MoonAirship::OnCollision(Collider* collider)
{
	if (state != HURT)
	{
		if (--hitpoints == 0) {
			App->particles->AddParticle(BIG_EXPLOSION, position.x - 30, position.y - 20);
			App->player->AddScore(150, collider->type);
		}

		else
			state = HURT;
	}

}


