#include "Application.h"
#include "Enemy_BonusAirship.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_BonusAirship::Enemy_BonusAirship(int x, int y) : Enemy(x, y)
{
	fly.SetUp(200, 0, 105, 95, 1, 1, "0");
	fly.speed = 0.2f;

	path.PushBack({ 0, 3 }, 30, &fly);
	path.PushBack({ 0, -2 }, 80, &fly);
	path.PushBack({ 0,-5 }, 500, &fly);

	collider = App->collision->AddCollider({ 200, 0, 105, 95 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	original_y = y;
}

void Enemy_BonusAirship::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}
