#include "Application.h"
#include "Enemy.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

Enemy::Enemy(int x, int y) : position(x, y)
{
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
	animation_hurt.CleanUp();
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	switch (state) {
	case REGULAR:
		if (animation != nullptr)
			App->render->Blit(sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
		break;
	case HURT:
		App->render->Blit(sprites, position.x, position.y, direction, &(animation_hurt.GetCurrentFrame()));
		if (animation_hurt.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;
	}
}

void Enemy::OnCollision(Collider* collider)
{
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(EXPLOSION, position.x, position.y);
			App->player->AddScore(50);
		}
		else
			state = HURT;
	}
	
}

void Enemy::Shoot(iPoint origin)
{
	iPoint player_position = App->player->GetPos();
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { (float)player_position.x - position.x,  (float)player_position.y - origin.y});
}