#include "Application.h"
#include "Enemy_LightTank.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"

Enemy_LightTank::Enemy_LightTank(int x, int y, int subtype) : Enemy(x, y)
{
	walk.SetUp(0, 67, 34, 45, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	turret.SetUp(0, 112, 68, 69, 1, 1, "0");
	turret.speed = 0.2f;

	animation_shooting.SetUp(0, 67, 34, 44, 3, 3, "0,1,2");
	animation_shooting.speed = 0.3f;

	path.PushBack({ 0,0 }, 50, &walk);
	switch (subtype)
	{
	case NORMAL:
		path.PushBack({ 0,0 }, 150, &animation_shooting);
		path.PushBack({ 0,-0.5f }, 300, &walk);
		path.PushBack({ 0,0 }, 6000, &walk);
		path.IsFinished();
		break;

	case VARIATION1:
		path.PushBack({ 0,0 }, 200, &walk);
		break;

	case VARIATION2:
		Enemy::direction = { 1, 0 };
		if(position.x > SCREEN_WIDTH / 2)
			path.PushBack({ -0.5f,0 }, 300, &walk);
		if (position.x < SCREEN_WIDTH / 2)
			path.PushBack({ 0.5f,0 }, 300, &walk);

		break;
	}
		
	path.LoopStart(50);
	

	collider = App->collision->AddCollider({ 0, 67, 25, 35 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	type = GROUND;
	hitpoints = 2;

	sdl_clock_start = SDL_GetTicks() + 2000 + 850;
}

void Enemy_LightTank::Draw(SDL_Texture* sprites)
{
	App->collision->SetPosition(collider, position.x + 5, position.y + 6);

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

	if(hitpoints > 1){
		if (sdl_clock >= sdl_clock_start && position.y - App->render->camera.y <= SCREEN_HEIGHT * 5 / 8) {
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
}

void Enemy_LightTank::OnCollision(Collider* collider) {
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(EXPLOSION, position.x, position.y);
			App->audio->PlaySFX(App->particles->explosion.fx);
			App->particles->AddParticle(CRATER, position.x + x_offset, position.y + y_offset);
			App->player->AddScore(150, collider->type);
		}
		else if (hitpoints == 1) {
			App->particles->AddParticle(EXPLOSION, position.x + x_offset, position.y + y_offset);
			state = HURT;
		}

		else
			state = HURT;
	}

}