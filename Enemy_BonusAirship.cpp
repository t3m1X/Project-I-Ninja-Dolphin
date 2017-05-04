#include "Application.h"
#include "Enemy_BonusAirship.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_BonusAirship::Enemy_BonusAirship(int x, int y) : Enemy(x, y)
{
	fly.SetUp(0, 253, 150, 95, 1, 1, "0");
	fly.speed = 0.2f;

	animation_hurt.SetUp(0, 253, 150, 95, 2, 2, "1,0,1,0,1");

	direction = App->player->GetPos() - position;
	fPoint fdirection = { (float)direction.x,(float)direction.y };
	fdirection.Normalize();

	path.PushBack(fdirection * 3, 500, &fly);

	collider = App->collision->AddCollider({ 200, 0, 105, 95 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	hitpoints = 3;
}

Enemy_BonusAirship::~Enemy_BonusAirship()
{
	fly.CleanUp();
}

void Enemy_BonusAirship::Move()
{

	position = original_position + path.GetCurrentPosition(&animation);
}
