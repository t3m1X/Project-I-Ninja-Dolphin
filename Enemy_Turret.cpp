#include "Application.h"
#include "Enemy_Turret.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_Turret::Enemy_Turret(int x, int y) : Enemy(x, y)
{
	walk.SetUp(102, 67, 49, 43, 1, 1, "0");
	walk.speed = 0.2f;

	animation_shooting.SetUp(152, 67, 49, 43, 3, 3,"0,1,2");
	animation_shooting.speed = 0.3f;

	animation_hurt.SetUp(0, 181, 44, 68, 4, 4, "3,0,3,0,3");
	shadow.SetUp(176, 215, 22, 35, 1, 1, "0");

	path.PushBack({ 0, 0 }, 1000, &walk);
	

	collider = App->collision->AddCollider({ 0, 67, 49, 43}, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	type = GROUND;
	hitpoints = 3;

	sdl_clock_start = SDL_GetTicks() + 1000;

}

Enemy_Turret::~Enemy_Turret()
{
	walk.CleanUp();
}

void Enemy_Turret::Move()
{
	sdl_clock = SDL_GetTicks();
	position = original_position + path.GetCurrentPosition(&animation);

	if (sdl_clock >= sdl_clock_start)
	{
		shots++;
		iPoint origin = position;
		origin.x += 18;
		origin.y += walk.CurrentFrame().h;
		Shoot(origin);
		shot = true;

		if (shots >= 1) {
			sdl_clock_start = sdl_clock + 3167;
			shots = 0;
		}
	}
}



void Enemy_Turret::OnCollision(Collider* collider) {

	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(EXPLOSION, position.x, position.y);
			App->particles->AddParticle(CRATER, position.x, position.y);
			App->player->AddScore(50);
		}
		
		}

		else
			state = HURT;
	}



