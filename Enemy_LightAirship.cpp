#include "Application.h"
#include "Enemy_LightAirship.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_LightAirship::Enemy_LightAirship(int x, int y) : Enemy(x, y)
{
	fly.SetUp(0, 0, 50, 67, 4, 4, "0,1,2,3");
	fly.speed = 0.2f;

	direction = App->player->GetPos() - position;
	fPoint fdirection = { (float)direction.x,(float)direction.y };
	fdirection.Normalize();

	path.PushBack(fdirection*3, 20, &fly);
	path.PushBack({ 0, -2 }, 40, &fly);

	collider = App->collision->AddCollider({ 0, 0, 50, 67 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	sdl_clock_start = SDL_GetTicks();

	original_y = y;

	type = AIRBORNE;
}

Enemy_LightAirship::~Enemy_LightAirship()
{
	fly.CleanUp();
}

void Enemy_LightAirship::Move()
{
	if (!shot) {
		direction = App->player->GetPos() - position;
	}

	sdl_clock = SDL_GetTicks();
	position = original_position + path.GetCurrentPosition(&animation);

	if (sdl_clock >= sdl_clock_start + 400 && !shot) {
		fPoint fdirection = { (float)direction.x, (float)direction.y };
		fdirection.Normalize();
		path.PushBack(fdirection*-5, 500, &fly);
		iPoint origin = position;
		origin.x += 18;
		origin.y += fly.CurrentFrame().h;
		Shoot(origin);
		origin.x += 17;
		Shoot(origin);
		shot = true;
	}
}