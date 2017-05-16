#include "Application.h"
#include "Enemy_Box.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleBonus.h"

Enemy_Box::Enemy_Box(int x, int y) : Enemy(x, y)
{
	walk.SetUp(0, 181, 44, 68, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	
	

	path.PushBack({ 0,0 }, 1000, &walk);
	/*path.PushBack({ 0,-1.5f }, 300, &walk);*/

	collider = App->collision->AddCollider({ 0, 190, 45, 70 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

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
			App->bonus->AddBonus(MEDAL_BONUS, position.x, position.y);
			App->particles->AddParticle(EXPLOSION, position.x - 40, position.y - 10);
			App->player->AddScore(50);

		}


	}
}