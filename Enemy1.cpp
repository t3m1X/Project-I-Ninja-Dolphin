#include "Application.h"
#include "Enemy1.h"
#include "ModuleCollision.h"

Enemy1::Enemy1(int x, int y) : Enemy(x, y)
{
	fly.PushBack({ 0, 0, 50, 67 });
	fly.PushBack({ 50, 0, 50, 67 });
	fly.PushBack({ 100, 0, 50, 67 });
	fly.PushBack({ 150, 0, 50, 67 });
	fly.speed = 0.2f;

	animation = &fly;

	collider = App->collision->AddCollider({ 0, 0, 50, 67 }, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);

	original_y = y;
}

void Enemy1::Move()
{
	if (going_up)
	{
		if (wave > 1.0f)
			going_up = false;
		else
			wave += 0.05f;
	}
	else
	{
		if (wave < -1.0f)
			going_up = true;
		else
			wave -= 0.05f;
	}

	position.y = original_y + (25.0f * sinf(wave));
	position.x -= 1;
}
