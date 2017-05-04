#include "Application.h"
#include "Enemy_Proyectil.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_Proyectil::Enemy_Proyectil(int x, int y) : Enemy(x, y)
{
	walk.SetUp(0, 181, 63, 71, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	animation_hurt.SetUp(0, 181, 63, 71, 4, 4, "3,0,3,0,3");

	path.PushBack({ 0, 0.5f }, 100, &walk);
	

	collider = App->collision->AddCollider({ 0, 190, 45, 70 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	hitpoints = 3;

}

Enemy_Proyectil::~Enemy_Proyectil()
{
	walk.CleanUp();
}

void Enemy_Proyectil::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}

void Enemy_Proyectil::Damaged()
{
	damage.SetUp(196, 182, 55, 70, 1, 1, "0");
}

void Enemy_Proyectil::OnCollision(Collider* collider) {
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(EXPLOSION, position.x, position.y);
			App->player->AddScore(50);

			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 1,0 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { -1,0 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 0,1 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 0,-1 });

			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 1,1 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { -1,1 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 1,-1 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { -1,-1 });

		}
		else
			state = HURT;
	}

}