#include "Application.h"
#include "Enemy_BonusAirship.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_BonusAirship::Enemy_BonusAirship(int x, int y) : Enemy(x, y)
{
	fly.speed = 0.2f;

	direction = App->player->GetPos() - position;
	fPoint fdirection = { (float)direction.x,(float)direction.y };
	fdirection.Normalize();

	path.PushBack(fdirection * 3, 500, &fly);

	collider = App->collision->AddCollider({ 200, 0, 105, 95 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	original_y = y;
}

Enemy_BonusAirship::~Enemy_BonusAirship()
{
	fly.CleanUp();
}

void Enemy_BonusAirship::Move()
{

	position = original_position + path.GetCurrentPosition(&animation);
}
