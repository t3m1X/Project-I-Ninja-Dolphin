#include "Application.h"
#include "Enemy_Boss.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

Enemy_Boss::Enemy_Boss(int x, int y, int subtype) : Enemy(x, y)
{

	switch (subtype)
	{
	case TYPE1:
	{

		walk.SetUp(0, 381, 156, 154, 3, 3, "0,1,2");
		walk.speed = 0.2f;


		animation_shooting.SetUp(0, 535, 156, 154, 3, 3, "0,1,2");
		animation_shooting.speed = 0.2f;
		animation_shooting.loop = false;

		animation_hurt.SetUp(0, 381, 156, 154, 4, 4, "2,3,2,3,2");

		turret.SetUp(102, 67, 49, 43, 1, 1, "0");

		path.PushBack({ 0,-0.5f }, 100, &walk);
		path.IsFinished();
		break;
	}
	case TYPE2:
	{

		walk.SetUp(0, 732, 156, 154, 3, 3, "0,1,2");
		walk.speed = 0.2f;

		animation_shooting.SetUp(0, 886, 156, 154, 3, 3, "0,1,2");
		animation_shooting.speed = 0.2f;
		animation_shooting.loop = false;

		animation_hurt.SetUp(0, 732, 156, 154, 4, 4, "2,3,2,3,2");

		turret.SetUp(629, 590, 49, 43, 1, 1, "0");

		path.PushBack({ 1,0 }, 100, &walk);
		path.PushBack({ -1,0 }, 100, &walk);
		break;
	}

	}
	
	

	collider = App->collision->AddCollider({ 200, 0, 95, 70 }, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;


	sdl_clock_start = SDL_GetTicks() + 2000;

	type = GROUND;
	hitpoints = 20;
}

Enemy_Boss::~Enemy_Boss()
{
	walk.CleanUp();
	turret.CleanUp();
}


void Enemy_Boss::Draw(SDL_Texture * sprites)
{
	App->collision->SetPosition(collider, position.x, position.y);
	iPoint fdirection = App->player->GetPos() - position;

	

	switch (state) {
	case REGULAR:
		if (animation != nullptr) {
			App->render->Blit(type, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
			App->render->Blit(4, sprites, position.x + 53, position.y + 63, fdirection, &(turret.GetCurrentFrame()));
		}
		break;

	case SHOOTING:
		App->render->Blit(type, sprites, position.x, position.y, direction, &(animation_shooting.GetCurrentFrame()));
		App->render->Blit(4, sprites, position.x + 53, position.y + 63, fdirection, &(turret.GetCurrentFrame()));
		if (animation_shooting.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;

	case HURT:
		App->render->Blit(type, sprites, position.x, position.y, direction, &(animation_hurt.GetCurrentFrame()));
		App->render->Blit(4, sprites, position.x + 53, position.y + 63, fdirection, &(turret.GetCurrentFrame()));
		if (animation_hurt.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;
	}

}


void Enemy_Boss::Move()
{

	position = original_position + path.GetCurrentPosition(&animation);

	sdl_clock = SDL_GetTicks();

	if (sdl_clock >= sdl_clock_start) {
		shots++;
		iPoint origin = position;
		origin.x += 10;
		origin.y += walk.CurrentFrame().h -20;
		Shoot(origin, LOW_LEFT_DIR);
		origin.y += walk.CurrentFrame().h -210;
		Shoot(origin, LEFT_DIR);
		origin.y += walk.CurrentFrame().h - 190;
		Shoot(origin, UP_LEFT_DIR);
		origin.x += 120;
		origin.y += walk.CurrentFrame().h - 90;
		Shoot(origin, LOW_RIGHT_DIR);
		origin.y += walk.CurrentFrame().h - 190;
		Shoot(origin, RIGHT_DIR);
		origin.y += walk.CurrentFrame().h - 190;
		Shoot(origin, UP_RIGHT_DIR);
		origin.x += -50;
		origin.y += walk.CurrentFrame().h - 90;
		Shoot(origin, PLAYER_DIR);

		if (shots >= 1) {
			sdl_clock_start = sdl_clock + 600;
		}
	}

}

void Enemy_Boss::Shoot(iPoint origin, SHOT_DIR typology)
{
	state = SHOOTING;
	animation_shooting.Reset();
	
	
	switch (typology)
	{
	case SHOT_DIR::LOW_LEFT_DIR:
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
		break;
	case SHOT_DIR::LEFT_DIR:
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,0 });
		break;
	case SHOT_DIR::UP_LEFT_DIR:
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,-1 });
		break;
	case SHOT_DIR::LOW_RIGHT_DIR:
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,1 });
		break;
	case SHOT_DIR::RIGHT_DIR:
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,0 });
		break;
	case SHOT_DIR::UP_RIGHT_DIR:
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,-1 });
		break;
	case SHOT_DIR::PLAYER_DIR:
	{
		iPoint player_position = App->player->GetPos();
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { (float)player_position.x - position.x,  (float)player_position.y - origin.y });
		break;
	}

	}
	
}



void Enemy_Boss::OnCollision(Collider* collider)
{
	if (state != HURT)
	{
		if (--hitpoints == 0) {
			App->particles->AddParticle(BIG_EXPLOSION, position.x - 30, position.y - 20);
			App->player->AddScore(50);
		}

		else
			state = HURT;
	}

}
