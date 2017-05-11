#include "Application.h"
#include "Enemy_LightTank.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"

Enemy_LightTank::Enemy_LightTank(int x, int y, int subtype) : Enemy(x, y)
{
	walk.SetUp(0, 67, 34, 44, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	turret.SetUp(0, 112, 68, 69, 1, 1, "0");
	turret.speed = 0.2f;

	animation_shooting.SetUp(0, 67, 34, 44, 3, 3, "0,1,2");
	animation_shooting.speed = 0.3f;

	switch (subtype)
	{
	case NORMAL:
		path.PushBack({ 0, 1 }, 150, &walk);
		path.PushBack({ 0,0 }, 100, &animation_shooting);
		path.PushBack({ 0,-0.5f }, 6000, &walk);
		path.IsFinished();
		break;

	case VARIATION1:
		path.PushBack({ 0,0 }, 200, &walk);
		break;

	case VARIATION2:
		path.PushBack({ -0.5f,0 }, 300, &walk);
		break;
	}
		
	
	

	collider = App->collision->AddCollider({ 0, 67, 33, 44 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	type = GROUND;
	hitpoints = 2;

	sdl_clock_start = SDL_GetTicks() + 2000;
}

void Enemy_LightTank::Draw(SDL_Texture* sprites)
{
	App->collision->SetPosition(collider, position.x, position.y);

 	switch (state) {
	case HURT:
	case REGULAR:
		if (animation != nullptr)
			App->render->Blit(type, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
		if (hitpoints == 2) {
			x_offset = animation->CurrentFrame().w / 2 - turret.CurrentFrame().w / 2;
			y_offset = animation->CurrentFrame().h / 2 - turret.CurrentFrame().h / 2;
			iPoint turret_direction = App->player->GetPos() - position;
			App->render->Blit(type, sprites, position.x + x_offset, position.y + y_offset, turret_direction, &(turret.GetCurrentFrame()));
		}
		state = REGULAR;
		break;

	case SHOOTING:
		if (animation != nullptr)
			App->render->Blit(type, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
		if (hitpoints == 2) {
			x_offset = animation->CurrentFrame().w / 2 - turret.CurrentFrame().w / 2;
			y_offset = animation->CurrentFrame().h / 2 - turret.CurrentFrame().h / 2;
			iPoint turret_direction = App->player->GetPos() - position;
			App->render->Blit(type, sprites, position.x + x_offset, position.y + y_offset, turret_direction, &(turret.GetCurrentFrame()));
		}
		state = REGULAR;
	}
}

Enemy_LightTank::~Enemy_LightTank()
{
	walk.CleanUp();
	turret.CleanUp();
}

void Enemy_LightTank::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);

	sdl_clock = SDL_GetTicks();
	position = original_position + path.GetCurrentPosition(&animation);

	if (sdl_clock >= sdl_clock_start) {
	/*	fPoint fdirection = { (float)direction.x, (float)direction.y };
		fdirection.Normalize();
		path.PushBack(fdirection*-5, 500, &walk);*/
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

void Enemy_LightTank::OnCollision(Collider* collider) {
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(EXPLOSION, position.x, position.y);
			App->audio->PlaySFX(App->particles->explosion.fx);
			App->particles->AddParticle(CRATER, position.x + x_offset, position.y + y_offset);
			App->player->AddScore(50);
		}
		else if (hitpoints == 1) {
			App->particles->AddParticle(EXPLOSION, position.x + x_offset, position.y + y_offset);
			state = HURT;
		}

		else
			state = HURT;
	}

}