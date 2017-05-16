#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "ModuleBonus.h"
#include "ModulePlayer.h"
#include <cstdlib>

ModuleBonus::ModuleBonus()
{
	for (uint i = 0; i < MAX_BONUS; ++i)
		bonus[i] = nullptr;
}

// Destructor
ModuleBonus::~ModuleBonus()
{
}

bool ModuleBonus::Start()
{
	sprites = App->textures->Load("revamp_spritesheets/UpgradeBeacon.png");

	red_bonus.SetUp(0, 0, 30, 26, 8, 8, "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,5,6,7");
	red_bonus.speed = 0.2f;

	blue_bonus.SetUp(0, 26, 30, 26, 8, 8, "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7");
	blue_bonus.speed = 0.2f;
	
	medal_bonus.SetUp(0, 78, 19, 33, 6, 6, "0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5");
	medal_bonus.speed = 0.2f;

	return true;
}

// Called before render is available
update_status ModuleBonus::Update()
{
	for (uint i = 0; i < MAX_BONUS; ++i)
		if (bonus[i] != nullptr) bonus[i]->Update();

	for (uint i = 0; i < MAX_BONUS; ++i)
		if (bonus[i] != nullptr) {
			Animation* print = nullptr;
			switch (bonus[i]->type) {
			case RED_BONUS:
				print = &red_bonus;
				break;
			case BLUE_BONUS:
				print = &blue_bonus;
				break;
			case MISSILE_BONUS:
				print = &missile_bonus;
				break;
			case MEDAL_BONUS:
				print = &medal_bonus;
				break;
			}

			App->render->Blit(6, sprites, bonus[i]->position.x, bonus[i]->position.y, { 0,1 }, &print->GetCurrentFrame());
		}


	return UPDATE_CONTINUE;
}

update_status ModuleBonus::PostUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_BONUS; ++i)
	{
		if (bonus[i] != nullptr)
		{
			if (bonus[i]->position.y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("DeSpawning bonus at %d", bonus[i]->position.y * SCREEN_SIZE);
				delete bonus[i];
				bonus[i] = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}

bool ModuleBonus::CleanUp()
{
	LOG("Freeing all bonus");

	App->textures->Unload(sprites);
	blue_bonus.CleanUp();
	red_bonus.CleanUp();
	missile_bonus.CleanUp();
	medal_bonus.CleanUp();

	for (uint i = 0; i < MAX_BONUS; ++i)
	{
		if (bonus[i] != nullptr)
		{
			delete bonus[i];
			bonus[i] = nullptr;
		}
	}

	return true;
}

bool ModuleBonus::AddBonus(BONUS_TYPE type, int x, int y)
{
	bool ret = false;

	uint i = 0;
	for (; bonus[i] != nullptr && i < MAX_BONUS; ++i);

	if (i != MAX_BONUS) {
		switch (type) {
		case BONUS_TYPE::MEDAL_BONUS:
			bonus[i] = new PowerUp(x, y, type);
			bonus[i]->col = App->collision->AddCollider(SDL_Rect{ x,y,19,33 }, COLLIDER_BONUS, this);
			break;
		case BONUS_TYPE::MISSILE_BONUS:
		case BONUS_TYPE::BLUE_BONUS:
		case BONUS_TYPE::RED_BONUS:
			bonus[i] = new PowerUp(x, y, type);
			bonus[i]->col = App->collision->AddCollider(SDL_Rect{ x,y,30,26 }, COLLIDER_BONUS, this);
			break;
		}
	}

	return ret;
}

void ModuleBonus::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_BONUS; ++i)
	{
		if (bonus[i] != nullptr && bonus[i]->col == c1)
		{
			App->player->AddBonus(bonus[i]->type, c2);
			delete bonus[i];
			bonus[i] = nullptr;
			break;
		}
	}
}


Bonus::~Bonus()
{
	App->collision->EraseCollider(col);
}

void PowerUp::Update() {
	int sdl_clock = SDL_GetTicks();

	if (type == BLUE_BONUS && sdl_clock > sdl_clock_next) {
		type = RED_BONUS;
		sdl_clock_next = sdl_clock + 3000;

		new_pos.x = (App->render->camera.x + SCREEN_WIDTH / 2) - bonus_position.x;
		new_pos.y = (App->render->camera.y + SCREEN_HEIGHT / 2 - 100) - bonus_position.y;
	}

	else if (type == RED_BONUS && sdl_clock > sdl_clock_next) {
		type = BLUE_BONUS;
		sdl_clock_next = sdl_clock + 3000;
	}
	srand(SDL_GetTicks());

	float factor = (float)M_PI / 180.0f;
	
	int radius = 100;

	
	if (new_pos.x != 0) {
		int factor = new_pos.x / abs(new_pos.x);
		bonus_position.x += factor /** 2*/;
		new_pos.x -= factor/* * 2*/;
	}
	if (new_pos.y != 0) {
		int factor = new_pos.y / abs(new_pos.y);
		bonus_position.y += factor * 2;
		new_pos.y -= factor /** 2*/;
	}
	else
		bonus_position.y -= SCROLL_SPEED;

	position.x = (int)(bonus_position.x + radius * cos(circle_iterations * factor));
	position.y = (int)(bonus_position.y + radius * sin(circle_iterations * factor));

	App->collision->SetPosition(col, position.x, position.y);

	if (++circle_iterations > 360)
		circle_iterations = 0;
}
