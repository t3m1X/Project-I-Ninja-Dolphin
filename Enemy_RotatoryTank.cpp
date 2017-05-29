#include "Application.h"
#include "Enemy_RotatoryTank.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"


Enemy_RotatoryTank::Enemy_RotatoryTank(int x, int y) : Enemy(x, y)
{
	walk.SetUp(629, 635, 94, 93, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	turret.SetUp(629, 729, 43, 40, 1, 1, "0");

	turret_shooting.SetUp(673, 729, 43, 40, 1, 1, "0");
	turret_shooting.speed = 0.2f;
	turret_shooting.loop = false;


	animation_hurt.SetUp(629, 635, 94, 93, 4, 4, "2,3,2,3,2");

	if (position.x > SCREEN_WIDTH / 2)
	{
		path.PushBack({ 0,0 }, 100, &walk);
		path.PushBack({ -1,0 }, 100, &walk);
		path.PushBack({ 0,-1 }, 50, &walk);
		path.PushBack({ 0,0 }, 50, &walk);
		path.PushBack({ 0,-1.2f }, 250, &walk);
		path.LoopStart(250);
	}

	if (position.x < SCREEN_WIDTH / 2)
	{
		path.PushBack({ 0,0 }, 100, &walk);
		path.PushBack({ 1,0 }, 100, &walk);
		path.PushBack({ 0,-1 }, 50, &walk);
		path.PushBack({ 0,0 }, 50, &walk);
		path.PushBack({ 0,-1.2f }, 250, &walk);
		path.LoopStart(250);
	}

	collider = App->collision->AddCollider({ 200, 0, 105, 95 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	sdl_clock_start = SDL_GetTicks();


	type = GROUND;
	hitpoints = 10;
}

Enemy_RotatoryTank::~Enemy_RotatoryTank()
{
	walk.CleanUp();
	turret.CleanUp();
	turret_shooting.CleanUp();
}

void Enemy_RotatoryTank::Draw(SDL_Texture * sprites)
{
	App->collision->SetPosition(collider, position.x, position.y);
	

	switch (state) {
	case REGULAR:
		if (animation != nullptr) {
			App->render->Blit(type, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
			App->render->Blit(4, sprites, position.x + 20, position.y + 24, direction, &(turret.GetCurrentFrame()));
		}
		break;

	case SHOOTING:
		App->render->Blit(type, sprites, position.x, position.y, direction, &(animation->GetCurrentFrame()));
		App->render->Blit(4, sprites, position.x + 20, position.y + 24, direction, &(turret_shooting.GetCurrentFrame()));
		if (turret_shooting.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;

	case HURT:
		App->render->Blit(type, sprites, position.x, position.y, direction, &(animation_hurt.GetCurrentFrame()));
		App->render->Blit(4, sprites, position.x + 20, position.y + 24, direction, &(turret_shooting.GetCurrentFrame()));
		if (animation_hurt.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;
	}

}


void Enemy_RotatoryTank::Move()
{
	sdl_clock = SDL_GetTicks();
	position = original_position + path.GetCurrentPosition(&animation);
	iPoint origin = position;
	origin.x = origin.x + 40;
	origin.y = origin.y + walk.CurrentFrame().h - 54;

	if (sdl_clock >= sdl_clock_start + 2100) { 

		Shoot(origin, FIRST);
		Shoot(origin, SECOND);
		Shoot(origin, THIRD);
		Shoot(origin, FOURTH);
		
		sdl_clock_start = sdl_clock + 3167;
	}
	
}

void Enemy_RotatoryTank::Shoot(iPoint origin, ROUNDS typology)
{
	state = SHOOTING;
	animation_shooting.Reset();

	sdl_clock = SDL_GetTicks();

	switch (typology)
	{
	case FIRST: 
	{
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 0,1 });//primera tanda, de arriba a abajo
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 0,-1 });
	}
	break;
	case SECOND:
	{
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,-1 });//diagonal de izq a derecha
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,1 });
	}
	break;
	case THIRD:
	{
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,-1 });//diagonal de deerecha a izquiera
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,1 });
	}
	break;
	case FOURTH:
	{
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { 1,0 });//de izq a derecha
		App->particles->AddParticle(ENEMYSHOT, origin.x, origin.y, { -1,0 });
	}
	break;

	}

	/*sdl_clock_start = sdl_clock + 3167;*/
}

void Enemy_RotatoryTank::OnCollision(Collider* collider)
{
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(BIG_EXPLOSION, position.x, position.y);
			App->player->AddScore(50);
		}
		else
			state = HURT;
	}
}
