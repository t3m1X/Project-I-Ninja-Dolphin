#include "Application.h"
#include "Enemy1.h"
#include "ModuleCollision.h"

Enemy1::Enemy1(int x, int y) : Enemy(x, y)
{
	fly.SetUp(0, 0, 50, 67, 4, 4, "0,1,2,3");
	fly.speed = 0.2f;

	path.PushBack({ 0,5 }, 160, &fly);
	path.PushBack({ 0,-5 }, 500, &fly);

	collider = App->collision->AddCollider({ 0, 0, 50, 67 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	original_y = y;
}

void Enemy1::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}
