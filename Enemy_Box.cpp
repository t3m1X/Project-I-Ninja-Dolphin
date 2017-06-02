#include "Application.h"
#include "Enemy_Box.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleBonus.h"

Enemy_Box::Enemy_Box(int x, int y) : Enemy(x, y)
{
	walk.SetUp(0, 689, 43, 42, 3, 3, "0,1,2");
	walk.speed = 0.1f;

	
	

	path.PushBack({ 0,0 }, 1000, &walk);
	

	collider = App->collision->AddCollider({ 0, 190, 42, 40 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	type = GROUND;
	hitpoints = 1;

}

Enemy_Box::~Enemy_Box()
{
	walk.CleanUp();
}

void Enemy_Box::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}



void Enemy_Box::OnCollision(Collider* collider) {

	if (state != HURT) {
		if (--hitpoints == 0) {
			App->bonus->AddBonus(MEDAL_BONUS, position.x + 10, position.y);
			App->particles->AddParticle(EXPLOSION, position.x - 15, position.y - 10);
			App->player->AddScore(25, collider->type);

		}


	}
}