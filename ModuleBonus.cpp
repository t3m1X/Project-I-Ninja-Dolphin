#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "ModuleBonus.h"
#include "Bonus.h"
#include "Bonus_RedBonus.h"

#define SPAWN_MARGIN 100

ModuleBonus::ModuleBonus()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		bonus[i] = nullptr;
}

// Destructor
ModuleBonus::~ModuleBonus()
{
}

bool ModuleBonus::Start()
{
	
	sprites = App->textures->Load("revamp_spritesheets/UpgradeBeacon.png");

	return true;
}

update_status ModuleBonus::PreUpdate()//problemos con spawn margin
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_BONUS; ++i)
	{
		if (queue[i].type != BONUS_TYPE::NO_BONUS_TYPE)
		{
			if (queue[i].y * SCREEN_SIZE > App->render->camera.y * SCREEN_SIZE - SPAWN_MARGIN)
			{
				SpawnBonus(queue[i]);
				queue[i].type = BONUS_TYPE::NO_BONUS_TYPE;
				LOG("Spawning bonus at %d", queue[i].y * SCREEN_SIZE);
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleBonus::Update()
{
	for (uint i = 0; i < MAX_BONUS; ++i)
		if (bonus[i] != nullptr) bonus[i]->Move();

	for (uint i = 0; i < MAX_BONUS; ++i)
		if (bonus[i] != nullptr) bonus[i]->Draw(sprites);

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

	for (uint i = 0; i < MAX_BONUS; ++i)
	{
		if (queue[i].type == BONUS_TYPE::NO_BONUS_TYPE)
		{
			queue[i].type = type;
			queue[i].x = SCREEN_WIDTH / 2 + (x - STAGE_WIDTH / 2);
			queue[i].y = y - STAGE_HEIGHT + SCREEN_HEIGHT;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleBonus::SpawnBonus(const BonusInfo& info)
{
	// find room for the new bonus
	uint i = 0;
	for (; bonus[i] != nullptr && i < MAX_BONUS; ++i);

	if (i != MAX_BONUS)
	{
		switch (info.type)
		{
		case BONUS_TYPE::RED:
			bonus[i] = new Bonus_RedBonus(info.x, info.y);
			break;

		}
	}
}

void ModuleBonus::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_BONUS; ++i)
	{
		if (bonus[i] != nullptr && bonus[i]->GetCollider() == c1)
		{
			bonus[i]->OnCollision(c2);
			delete bonus[i];
			bonus[i] = nullptr;
			break;
		}
	}
}