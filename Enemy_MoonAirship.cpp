#include "Application.h"
#include "Enemy_MoonAirship.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"


Enemy_MoonAirship::Enemy_MoonAirship(int x, int y) : Enemy(x, y)
{
	fly.SetUp(629, 375, 97, 89, 4, 4, "0,1,2,3");
	fly.speed = 0.1f;

	shadow.SetUp(629, 555, 48, 32, 1, 1, "0");
	

	animation_shooting.SetUp(629, 465, 97, 89, 3, 3, "0,1,2");
	animation_shooting.speed = 0.2f;
	animation_shooting.loop = false;

	animation_hurt.SetUp(629, 465, 97, 89, 2, 2, "");

	path.PushBack({ 0, 0.5f }, 100, &fly);
	
	

	collider = App->collision->AddCollider({ 200, 0, 105, 95 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;
	


	type = AIRBORNE;
	hitpoints = 15;
}

Enemy_MoonAirship::~Enemy_MoonAirship()
{
	fly.CleanUp();
}

void Enemy_MoonAirship::Move()
{
	
	position = original_position + path.GetCurrentPosition(&animation);

	

}

