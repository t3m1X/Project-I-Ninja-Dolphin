#include "Application.h"
#include "Enemy_Kamikaze.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"

Enemy_Kamikaze::Enemy_Kamikaze(int x, int y) : Enemy(x, y)
{
	fly.SetUp(375, 108, 146, 108, 4, 4, "0,1,2,3");
	fly.speed = 0.2f;

	animation_shooting.SetUp(375, 0, 146, 108, 3, 3, "0,1,2");
	animation_shooting.speed = 0.2f;

	shadow.SetUp(959, 162, 73, 54, 1, 1, "0");

	animation_hurt.SetUp(375, 0, 146, 108, 5, 4, "3,4,3,4,3");
	
	

	path.PushBack({ 0,1 }, 200, &fly);

	if(position.x > (App->render->camera.x + SCREEN_WIDTH) / 2)
		path.PushBack({ -1,2 }, 150, &fly);
	else
		path.PushBack({ 1,2 }, 150, &fly);

	path.PushBack({ 0,-1 - SCROLL_SPEED }, 750, &fly);
	
	path.loop = false;

	collider = App->collision->AddCollider({ 518, 0, 146, 103 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;
	y_transition = position.y + 300;
	type = AIRBORNE;
	hitpoints = 20;

	sdl_clock_start = SDL_GetTicks();

}

Enemy_Kamikaze::~Enemy_Kamikaze()
{
	fly.CleanUp();
}

void Enemy_Kamikaze::Move()
{
	

	

	if (position.y >= y_transition)
		Enemy::direction = { 0,-1 };

	if (path.IsFinished())
		to_delete = true;
			
	position = original_position + path.GetCurrentPosition(&animation);
	sdl_clock = SDL_GetTicks();

	if (sdl_clock >= sdl_clock_start + 2000) { 

		iPoint origin = position;
		origin.x += 45;
		origin.y += fly.CurrentFrame().h;
		Shoot(origin);
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
		sdl_clock_start = sdl_clock + 3167;
	}
}

