#include "Application.h"
#include "Enemy.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"

Enemy::Enemy(int x, int y) : position(x, y)
{
	animation_hurt.loop = false;
	animation_hurt.speed = 0.4f;
	
	animation_shooting.loop = false;
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		App->collision->EraseCollider(collider);

	animation_hurt.CleanUp();
	animation_shooting.CleanUp();
	shadow.CleanUp();
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{

	App->collision->SetPosition(collider, position.x + collider_offset.x, position.y + collider_offset.y);
	
	switch (state) {
	case REGULAR:
		if (animation != nullptr)
			App->render->Blit(type, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
		break;

	case SHOOTING:
		App->render->Blit(type,sprites, position.x, position.y, direction, &(animation_shooting.GetCurrentFrame()));
		if (animation_shooting.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;

	case HURT:
		App->render->Blit(type, sprites, position.x, position.y, direction, &(animation_hurt.GetCurrentFrame()));
		if (animation_hurt.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;
	}

	if (type == AIRBORNE) {
		iPoint shadow_position = position + iPoint(animation->CurrentFrame().w / 2 + SHADOW_DISTANCE_X, animation->CurrentFrame().h / 2 + SHADOW_DISTANCE_Y);
		App->render->Blit(5, sprites, shadow_position.x, shadow_position.y, direction, &(shadow.GetCurrentFrame()));
	}

}

void Enemy::OnCollision(Collider* collider)
{
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(EXPLOSION, position.x, position.y);
			//App->audio->PlaySFX(App->particles->explosion.fx);
			App->player->AddScore(50);
		}
		else
			state = HURT;
	}
	
}

void Enemy::Shoot(iPoint origin)
{
	state = SHOOTING;
	animation_shooting.Reset();
	iPoint player_position = App->player->GetPos();
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { (float)player_position.x - position.x,  (float)player_position.y - origin.y});
}

