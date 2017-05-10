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
	
	sprites = App->textures->Load("revamp_spritesheets/UpgradeBeacon.png");

	return true;
}

// Called before render is available
update_status ModuleBonus::Update()
{
	for (uint i = 0; i < MAX_BONUS; ++i)
		if (bonus[i] != nullptr) bonus[i]->Update();

	for (uint i = 0; i < MAX_BONUS; ++i)
		if (bonus[i] != nullptr) bonus[i]->Draw(sprites);

	return UPDATE_CONTINUE;
}

update_status ModuleBonus::PostUpdate()
{
	//// check camera position to decide what to spawn
	//for (uint i = 0; i < MAX_BONUS; ++i)
	//{
	//	if (bonus[i] != nullptr)
	//	{
	//		if (bonus[i]->position.y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE) + SPAWN_MARGIN)
	//		{
	//			LOG("DeSpawning bonus at %d", bonus[i]->position.y * SCREEN_SIZE);
	//			delete[] bonus[i];
	//			bonus[i] = nullptr;
	//		}
	//	}
	//}

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
			delete[] bonus[i];
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
		case BONUS_TYPE::RED_BONUS:
			bonus[i] = new PowerUp(x, y, type);
			break;
		

		}
	}

	return ret;
}

void ModuleBonus::OnCollision(Collider* c1, Collider* c2)
{
	/*for (uint i = 0; i < MAX_BONUS; ++i)
	{
		if (bonus[i] != nullptr && bonus[i]->GetCollider() == c1)
		{
			bonus[i]->OnColl(c2);
			delete bonus[i];
			bonus[i] = nullptr;
			break;
		}
	}*/
}

void Bonus::OnColl(Collider * player) {
	App->player->AddBonus(type);
}

void Bonus::Draw(SDL_Texture* sprite)
{
	App->render->Blit(6, sprite, position.x, position.y, { 0, 1 }, &animation.GetCurrentFrame());
}

void PowerUp::Update() {
	float factor = (float)M_PI / 180.0f;
	int radius = 40;

	position.y -= SCROLL_SPEED;

	bonus_position.x = (int)(position.x + radius * cos(circle_iterations * factor));
	bonus_position.y = (int)(position.y + radius * sin(circle_iterations * factor));

	if (++circle_iterations > 360)
		circle_iterations = 0;
}
