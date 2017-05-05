#include "Application.h"
#include "Bonus.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

Bonus::Bonus(int x, int y) : position(x, y)
{
}

Bonus::~Bonus()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Bonus::GetCollider() const
{
	return collider;
}

void Bonus::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(6, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
}

void Bonus::OnCollision(Collider* collider)
{
	//App->particles->AddParticle(EXPLOSION, position.x, position.y);


}