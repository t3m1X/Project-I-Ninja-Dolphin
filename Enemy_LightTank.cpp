#include "Application.h"
#include "Enemy_LightTank.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_LightTank::Enemy_LightTank(int x, int y) : Enemy(x, y)
{
	walk.SetUp(0, 67, 34, 44, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	path.PushBack({ 0, 3 }, 25, &walk);
	path.PushBack({ 0, -2 }, 40, &walk);
	path.PushBack({ 0,-5 }, 500, &walk);

	collider = App->collision->AddCollider({ 0, 67, 33, 44 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;

	original_y = y;
}

Enemy_LightTank::~Enemy_LightTank()
{
	walk.CleanUp();
}

void Enemy_LightTank::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}