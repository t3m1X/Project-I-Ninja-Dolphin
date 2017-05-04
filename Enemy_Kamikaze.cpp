#include "Application.h"
#include "Enemy_Kamikaze.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_Kamikaze::Enemy_Kamikaze(int x, int y) : Enemy(x, y)
{
	fly.SetUp(518, 0, 146, 107, 1, 1, "0");
	fly.speed = 0.2f;

	/*direction = App->player->GetPos() - position;
	fPoint fdirection = { (float)direction.x,(float)direction.y };
	fdirection.Normalize();*/

	path.PushBack({ 0, 1.0f }, 100, &fly);

	collider = App->collision->AddCollider({ 518, 0, 146, 103 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	original_y = y;
}

Enemy_Kamikaze::~Enemy_Kamikaze()
{
	fly.CleanUp();
}

void Enemy_Kamikaze::Move()
{

	position = original_position + path.GetCurrentPosition(&animation);
}