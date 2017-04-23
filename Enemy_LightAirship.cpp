#include "Application.h"
#include "Enemy_LightAirship.h"
#include "ModuleCollision.h"

Enemy_LightAirship::Enemy_LightAirship(int x, int y) : Enemy(x, y)
{
	fly.SetUp(0, 0, 50, 67, 4, 4, "0,1,2,3");
	fly.speed = 0.2f;

	path.PushBack({ 0,5 }, 160, &fly);
	path.PushBack({ 0,-5 }, 500, &fly);

	collider = App->collision->AddCollider({ 0, 0, 50, 67 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	original_y = y;
}

void Enemy_LightAirship::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}
