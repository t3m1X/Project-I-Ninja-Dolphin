#include "Application.h"
#include "Enemy_Kamikaze.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_Kamikaze::Enemy_Kamikaze(int x, int y) : Enemy(x, y)
{
	fly.SetUp(518, 108, 146, 108, 4, 4, "0,1,2,3");
	fly.speed = 0.2f;

	animation_shooting.SetUp(518, 0, 146, 108, 3, 3, "0,1,2");
	animation_shooting.speed = 0.2;
	/*direction = App->player->GetPos() - position;
	fPoint fdirection = { (float)direction.x,(float)direction.y };
	fdirection.Normalize();*/

	path.PushBack({ 0, 1.0f }, 100, &fly);

	collider = App->collision->AddCollider({ 518, 0, 146, 103 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	type = AIRBORNE;
	hitpoints = 1;
}

Enemy_Kamikaze::~Enemy_Kamikaze()
{
	fly.CleanUp();
}

void Enemy_Kamikaze::Move()
{

	position = original_position + path.GetCurrentPosition(&animation);
}