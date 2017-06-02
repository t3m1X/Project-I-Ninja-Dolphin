#include "Application.h"
#include "Enemy_RotatoryTank.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"


Enemy_RotatoryTank::Enemy_RotatoryTank(int x, int y) : Enemy(x, y)
{
	walk.SetUp(629, 635, 94, 93, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	turret.SetUp(629, 729, 44, 46, 1, 1, "0");

	turret_shooting.SetUp(673, 729, 44, 46, 1, 1, "0");
	turret_shooting.speed = 0.2f;
	turret_shooting.loop = false;


	animation_hurt.SetUp(629, 635, 94, 93, 4, 4, "2,3,2,3,2");

	if (position.x > SCREEN_WIDTH / 2)
	{
		path.PushBack({ 0,0 }, 100, &walk);
		path.PushBack({ -1,0 }, 100, &walk);
		path.PushBack({ 0,-1 }, 50, &walk);
		path.PushBack({ 0,0 }, 50, &walk);
		path.PushBack({ 0,-1.2f }, 120, &walk);
		path.PushBack({ 0,0 }, 2000, &walk);
		/*path.LoopStart(250);*/
	}

	if (position.x < SCREEN_WIDTH / 2)
	{
		path.PushBack({ 0,0 }, 100, &walk);
		path.PushBack({ 1,0 }, 100, &walk);
		path.PushBack({ 0,-1 }, 50, &walk);
		path.PushBack({ 0,0 }, 50, &walk);
		path.PushBack({ 0,-1.2f }, 120, &walk);
		path.PushBack({ 0,0 }, 2000, &walk);
		/*path.LoopStart(250);*/
	}

	collider_offset.x = 7;
	collider_offset.y = 17;

	collider = App->collision->AddCollider({ 200, 0, 70, 55 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;

	sdl_clock_start = SDL_GetTicks();
	sdl_clock = sdl_clock_start + 2000;


	type = GROUND;
	hitpoints = 10;
}

Enemy_RotatoryTank::~Enemy_RotatoryTank()
{
	walk.CleanUp();
	turret.CleanUp();
	turret_shooting.CleanUp();
}

void Enemy_RotatoryTank::Draw(SDL_Texture * sprites)
{
	App->collision->SetPosition(collider, position.x + collider_offset.x, position.y + collider_offset.y);
	
	iPoint turret_direction;
	float factor = M_PI / 180;
	turret_direction.x = (100 * cos((turret_angle - 90) * factor));
	turret_direction.y = (100 * -sin((turret_angle - 90) * factor));

	if (rotating) {
		turret_angle -= 12;
		if (turret_angle <= -360) {
			rotating = false;
			turret_angle = 0;
		}
	}

	switch (state) {
	case REGULAR:
		if (animation != nullptr) {
			App->render->Blit(type, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
			App->render->Blit(type, sprites, position.x + 20, position.y + 20, turret_direction, &(turret.GetCurrentFrame()));
		}
		break;

	case SHOOTING:
		rotating = true;
		App->render->Blit(type, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
		App->render->Blit(type, sprites, position.x + 20, position.y + 20, turret_direction, &(turret_shooting.GetCurrentFrame()));
		if (turret_shooting.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;

	case HURT:
		App->render->Blit(type, sprites, position.x, position.y, direction, &(animation_hurt.GetCurrentFrame()));
		App->render->Blit(type, sprites, position.x + 20, position.y + 20, turret_direction, &(turret_shooting.GetCurrentFrame()));
		if (animation_hurt.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;
	}

}


void Enemy_RotatoryTank::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);

	iPoint origin = position;
	origin.x = origin.x + 40;
	origin.y = origin.y + walk.CurrentFrame().h - 54;
	
	float factor = (float)M_PI / 180.0f;

	if (shot_angle <= 720 && SDL_GetTicks() >= sdl_clock) {
		int radius = 1;
		fPoint shot_vector;
		shot_vector.x = (radius * cos((shot_angle - 90) * factor));
		shot_vector.y = (radius * sin((shot_angle - 90) * factor));
		LOG("Direction x: %f, Direction y: %f", shot_vector.x, shot_vector.y);
		Shoot(origin, shot_vector);
		shot_angle += 45;
	}
}

void Enemy_RotatoryTank::Shoot(iPoint origin, fPoint direction)
{
	state = SHOOTING;
	turret_shooting.Reset();

	sdl_clock = SDL_GetTicks() + 150;
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, direction);
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, direction * -1);

	/*sdl_clock_start = sdl_clock + 3167;*/
}

void Enemy_RotatoryTank::OnCollision(Collider* collider)
{
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(BIG_EXPLOSION, position.x - 25, position.y - 40);
			App->particles->AddParticle(CRATER, position.x + 5, position.y + 5);
		}
		else
			state = HURT;
	}
}
