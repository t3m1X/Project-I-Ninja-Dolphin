#include "Application.h"
#include "Bonus_RedBonus.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Bonus_RedBonus::Bonus_RedBonus(int x, int y) : Bonus(x, y)
{
	fly.SetUp(0, 0, 29, 25, 1, 1, "0");
	fly.speed = 0.2f;

	direction = App->player->GetPos() - position;
	fPoint fdirection = { (float)direction.x,(float)direction.y };
	fdirection.Normalize();

	path.PushBack(fdirection * 3, 20, &fly);
	path.PushBack({ 0, -2 }, 40, &fly);

	collider = App->collision->AddCollider({ 0, 0, 29, 25 }, COLLIDER_TYPE::COLLIDER_BONUS, (Module*)App->enemies);

	original_position = position;

	sdl_clock_start = SDL_GetTicks();

	original_y = y;
}

Bonus_RedBonus::~Bonus_RedBonus()
{
	fly.CleanUp();
}

void Bonus_RedBonus::Move()
{
	/*if (!shot) {
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
	}*/
}