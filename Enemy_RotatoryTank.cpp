#include "Application.h"
#include "Enemy_RotatoryTank.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"


Enemy_RotatoryTank::Enemy_RotatoryTank(int x, int y) : Enemy(x, y)
{
	walk.SetUp(629, 635, 93, 93, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	animation_shooting.SetUp(210, 253, 105, 123, 3, 3, "0,1,2");
	animation_shooting.speed = 0.2f;
	animation_shooting.loop = false;

	animation_hurt.SetUp(629, 635, 93, 93, 4, 4, "2,3,2,3,2");

	if (position.x > SCREEN_WIDTH / 2)
	{
		path.PushBack({ 0,0 }, 100, &walk);
		path.PushBack({ -1,0 }, 100, &walk);
		path.PushBack({ 0,1 }, 50, &walk);
		path.PushBack({ 0,0 }, 100, &walk);
		path.PushBack({ 0,1 }, 250, &walk);
	}

	if (position.x < SCREEN_WIDTH / 2)
	{
		path.PushBack({ 0,0 }, 100, &walk);
		path.PushBack({ 1,0 }, 100, &walk);
		path.PushBack({ 0,1 }, 50, &walk);
		path.PushBack({ 0,0 }, 100, &walk);
		path.PushBack({ 0,1 }, 250, &walk);
	}

	collider = App->collision->AddCollider({ 200, 0, 105, 95 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	sdl_clock_start = SDL_GetTicks();


	type = GROUND;
	hitpoints = 12;
}

Enemy_RotatoryTank::~Enemy_RotatoryTank()
{
	walk.CleanUp();
}

void Enemy_RotatoryTank::Move()
{
	sdl_clock = SDL_GetTicks();
	position = original_position + path.GetCurrentPosition(&animation);

	if (sdl_clock >= sdl_clock_start + 2100 && !has_transitioned) { //seconds that the ship takes to shoot

		iPoint origin = position;
		origin.x += 45;
		origin.y += walk.CurrentFrame().h;
		Shoot(origin);
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 0,1 });
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,1 });
		sdl_clock_start = sdl_clock + 3167;
	}

}

void Enemy_RotatoryTank::Shoot(iPoint origin)
{
	state = SHOOTING;
	animation_shooting.Reset();

	sdl_clock = SDL_GetTicks();

	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 0,1 });
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,1 });
	sdl_clock_start = sdl_clock + 3167;
}

void Enemy_RotatoryTank::OnCollision(Collider* collider)
{
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(BIG_EXPLOSION, position.x, position.y);
			App->player->AddScore(50);
		}
		else
			state = HURT;
	}
}
