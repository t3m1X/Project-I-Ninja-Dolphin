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
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
}

void Enemy::OnCollision(Collider* collider)
{
	App->particles->AddParticle(EXPLOSION, position.x, position.y);
	App->player->AddScore(50);
}

void Enemy::Shoot(iPoint origin)
{
	iPoint player_position = App->player->GetPos();
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { (float)player_position.x - position.x,  (float)player_position.y - origin.y});
	/*fPoint fdirection = { (float)direction.x, (float)direction.y };
	App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, fdirection);*/
}