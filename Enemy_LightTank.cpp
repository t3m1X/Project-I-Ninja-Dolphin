#include "Application.h"
#include "Enemy_LightTank.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"

Enemy_LightTank::Enemy_LightTank(int x, int y) : Enemy(x, y)
{
	walk.SetUp(0, 67, 34, 44, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	turret.SetUp(0, 112, 68, 69, 1, 1, "0");
	turret.speed = 0.2f;

	path.PushBack({ 0, 3 }, 50, &walk);
	path.PushBack({ 0, -2 }, 40, &walk);
	path.PushBack({ 0,-5 }, 500, &walk);

	collider = App->collision->AddCollider({ 0, 67, 33, 44 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	hitpoints = 2;

}

void Enemy_LightTank::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

 	switch (state) {
	case HURT:
	case REGULAR:
		if (animation != nullptr)
			App->render->Blit(sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
		if (hitpoints == 2) {
			int x_offset = animation->CurrentFrame().w / 2 - turret.CurrentFrame().w / 2;
			int y_offset = animation->CurrentFrame().h / 2 - turret.CurrentFrame().h / 2;
			iPoint turret_direction = App->player->GetPos() - position;
			App->render->Blit(sprites, position.x + x_offset, position.y + y_offset, turret_direction, &(turret.GetCurrentFrame()));
		}
		state = REGULAR;
		break;

	case SHOOTING:
		App->render->Blit(sprites, position.x, position.y, direction, &(animation_shooting.GetCurrentFrame()));
		if (animation_shooting.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break; 
	}
}

Enemy_LightTank::~Enemy_LightTank()
{
	walk.CleanUp();
}

void Enemy_LightTank::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}