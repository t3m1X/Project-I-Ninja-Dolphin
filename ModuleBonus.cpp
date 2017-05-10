#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "ModuleBonus.h"
#include "ModulePlayer.h"

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
	red_bonus.SetUp(0, 0, 30, 26, 8, 8, "0,1,3,4,5,6,7");
	red_bonus.speed = 0.05f;

	blue_bonus.SetUp(0, 26, 30, 26, 8, 8, "0,1,2,3,4,5,6,7");
	blue_bonus.speed = 0.05f;
	sprites = App->textures->Load("revamp_spritesheets/UpgradeBeacon.png");

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
	}

	if (type == RED_BONUS && sdl_clock > sdl_clock_next) {
		type = BLUE_BONUS;
		sdl_clock_next = sdl_clock + 3000;
	}

	float factor = (float)M_PI / 180.0f;
	int radius = 40;

	int horizontal = (App->render->camera.x + SCREEN_WIDTH / 2) - position.x;
	if (horizontal != 0)
		horizontal /= abs(horizontal);

	bonus_position.y -= SCROLL_SPEED;
	bonus_position.x += horizontal;

	App->collision->SetPosition(col, bonus_position.x, bonus_position.y);

	position.x = (int)(bonus_position.x + radius * cos(circle_iterations * factor));
	position.y = (int)(bonus_position.y + radius * sin(circle_iterations * factor));

	if (++circle_iterations > 360)
		circle_iterations = 0;
}
