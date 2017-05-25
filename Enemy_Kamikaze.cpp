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

	if(position.x > (App->render->camera.x + SCREEN_WIDTH) / 2) // if kamikaze spawsn left of the window goes right, and biceversa
		path.PushBack({ -1,2 }, 125, &fly);
	else
		path.PushBack({ 1,2 }, 125, &fly);


	path.PushBack({ 0,-1 - SCROLL_SPEED }, 900, &fly);

	
	path.loop = false;

	collider_offset.x = 40;
	collider_offset.y = 30;

	collider = App->collision->AddCollider({ 518, 0, 70, 50 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;
	y_transition = position.y + 300;
	type = AIRBORNE;
	hitpoints = 20;

	sdl_clock_start = SDL_GetTicks() + 1500;

}

Enemy_Kamikaze::~Enemy_Kamikaze()
{
	fly.CleanUp();
}

void Enemy_Kamikaze::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
	sdl_clock = SDL_GetTicks();

	if (position.y - 50 >= y_transition)
	{
		Enemy::direction = { 0,-1 };
		has_transitioned = true;
		sdl_clock_start = sdl_clock + 9000;
	}

			
	if (!has_transitioned && sdl_clock >= sdl_clock_start) {

		shots++;
		iPoint origin = position;
		origin.x += 25;
		origin.y += fly.CurrentFrame().h - 30;
		Shoot(origin);
		origin.x += 90;
		Shoot(origin);
		
		
		if (shots >= 5) {
			sdl_clock_start = sdl_clock + 3167;
			shots = 0;
		}
		else 
			sdl_clock_start = sdl_clock + 50;
	}

	if (has_transitioned && sdl_clock >= sdl_clock_start)
	{
		state = SHOOTING;
		animation_shooting.Reset();
		iPoint origin = position;
		origin.x += 70;
		origin.y += fly.CurrentFrame().h - 50;
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 0,1 });
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,1 });
		sdl_clock_start = sdl_clock + 102167;
	}

	if (path.IsFinished())
		to_delete = true;
}

void Enemy_Kamikaze::OnCollision(Collider* collider)
{
	if (state != HURT)
	{
		if (--hitpoints == 0) {
			App->particles->AddParticle(BIG_EXPLOSION, position.x, position.y - 20);
			App->player->AddScore(50, collider->type);
			App->audio->PlaySFX(App->particles->big_explosion.fx);
		}

		else
			state = HURT;
	}

}
