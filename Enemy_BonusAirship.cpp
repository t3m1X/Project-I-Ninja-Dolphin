#include "Application.h"
#include "Enemy_BonusAirship.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleBonus.h"

Enemy_BonusAirship::Enemy_BonusAirship(int x, int y) : Enemy(x, y)
{
	fly.SetUp(0, 253, 105, 123, 1, 1, "0");
	fly.speed = 0.1f;

	acceleration.SetUp(578, 253, 105, 123, 4, 4, "0,1,2,3");
	acceleration.speed = 0.1f;
	

	animation_shooting.SetUp(210, 253, 105, 123, 3, 3, "0,1,2");
	animation_shooting.speed = 0.2f;
	animation_shooting.loop = false;

	animation_hurt.SetUp(0, 253, 105, 123, 2, 2, "1,0,1,0,1");

	shadow.SetUp(525, 327, 53, 48, 1, 1, "0");


	
	direction = { 0,1 };
	direction2 = { 1,-1 };
	direction3 = { -1,-1 };
	
	
	fPoint fdirection = { (float)direction.x,(float)direction.y };
	fdirection.Normalize();
	fPoint fdirection2 = { (float)direction2.x,(float)direction2.y };
	fdirection2.Normalize();
	fPoint fdirection3 = { (float)direction3.x,(float)direction3.y };
	fdirection3.Normalize();
	
	
	
	path.PushBack(fdirection *2, 20, &fly); // Arrives
	path.PushBack({ 0, - SCROLL_SPEED }, 30, &fly); //Stays for some time
	path.PushBack(fdirection2 * SCROLL_SPEED / 2, 80, &fly);//goes right
	path.PushBack(fdirection3 * SCROLL_SPEED / 2, 80, &fly);//goes left
	path.LoopStart(20);
	
	collider_offset.x = 32;
	collider_offset.y = 33;

	collider = App->collision->AddCollider({ 200, 0, 40, 75 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	sdl_clock_start = SDL_GetTicks();


	type = AIRBORNE;
	hitpoints = 8;
}

Enemy_BonusAirship::~Enemy_BonusAirship()
{
	fly.CleanUp();
	acceleration.CleanUp();
}

void Enemy_BonusAirship::Move()
{
	if (!has_transitioned && position.y - App->render->camera.y >= SCREEN_HEIGHT * 5 / 8)
	{
		path.PushBack({ 0,2 }, 100, &acceleration);
		has_transitioned = true;
	}

	sdl_clock = SDL_GetTicks();
	position = original_position + path.GetCurrentPosition(&animation);

	if (sdl_clock >= sdl_clock_start + 2100 && !has_transitioned) { //seconds that the ship takes to shoot
		
		iPoint origin = position;
		origin.x += 45;
		origin.y += fly.CurrentFrame().h;
		Shoot(origin);
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 0,1 });
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,1 });
		sdl_clock_start = sdl_clock + 3167;
	}

}

void Enemy_BonusAirship::Shoot(iPoint origin)
{
	state = SHOOTING;
	animation_shooting.Reset();

	sdl_clock = SDL_GetTicks();

	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 0,1 });
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,1 });
	sdl_clock_start = sdl_clock + 3167;
}

void Enemy_BonusAirship::OnCollision(Collider* collider)
{
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->bonus->AddBonus(RED_BONUS, position.x, position.y);
			App->particles->AddParticle(BIG_EXPLOSION, position.x, position.y);
			App->audio->PlaySFX(App->particles->big_explosion.fx);
		}
		else
			state = HURT;
	}
}
