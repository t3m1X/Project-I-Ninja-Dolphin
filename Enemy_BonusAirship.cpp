#include "Application.h"
#include "Enemy_BonusAirship.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_BonusAirship::Enemy_BonusAirship(int x, int y) : Enemy(x, y)
{
	fly.SetUp(0, 253, 105, 96, 1, 1, "0");
	fly.speed = 0.1f;

	animation_shooting.SetUp(210, 253, 105, 96, 3, 3, "0,1,2");
	animation_shooting.speed = 0.2f;

	animation_hurt.SetUp(0, 253, 105, 96, 2, 2, "1,0,1,0,1");

	shadow.SetUp(525, 301, 53, 48, 1, 1, "0");


	
	direction = { 0,1 };
	direction2 = { 1,0 };
	direction3 = { -1,0 };
	
	
	fPoint fdirection = { (float)direction.x,(float)direction.y };
	fdirection.Normalize();
	fPoint fdirection2 = { (float)direction2.x,(float)direction2.y };
	fdirection2.Normalize();
	fPoint fdirection3 = { (float)direction3.x,(float)direction3.y };
	fdirection3.Normalize();
	
	
	
	path.PushBack(fdirection *2, 20, &fly); // Arrives
	path.PushBack({ 0,0 }, 30, &fly); //Stays for some time
	path.PushBack(fdirection2 *0.5f, 80, &fly);//goes right
	path.PushBack(fdirection3 *0.5f, 80, &fly);//goes left
	
	collider = App->collision->AddCollider({ 200, 0, 105, 95 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	sdl_clock_start = SDL_GetTicks();

	original_y = y;

	type = AIRBORNE;
	hitpoints = 3;
}

Enemy_BonusAirship::~Enemy_BonusAirship()
{
	fly.CleanUp();
}

void Enemy_BonusAirship::Move()
{

	

	sdl_clock = SDL_GetTicks();
	position = original_position + path.GetCurrentPosition(&animation);

	if (sdl_clock >= sdl_clock_start + 2100 && !shot) { //seconds that the ship takes to shoot
		
		iPoint origin = position;
		origin.x += 45;
		origin.y += fly.CurrentFrame().h;
		Shoot(origin);
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,1 });
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
		shot = true;

		
	}

	
}
