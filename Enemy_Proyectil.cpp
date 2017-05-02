#include "Application.h"
#include "Enemy_Proyectil.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_Proyectil::Enemy_Proyectil(int x, int y) : Enemy(x, y)
{
	walk.SetUp(0, 181, 63, 71, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	path.PushBack({ 0, 0.5f }, 100, &walk);
	

	collider = App->collision->AddCollider({ 0, 67, 33, 44 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	original_y = y;
}

Enemy_Proyectil::~Enemy_Proyectil()
{
	walk.CleanUp();
}

void Enemy_Proyectil::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}

void Enemy_Proyectil::Damaged()
{
	damage.SetUp(196, 182, 55, 70, 1, 1, "0");
}