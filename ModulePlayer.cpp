#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"

ModulePlayer::ModulePlayer() 
{}

ModulePlayer::~ModulePlayer() 
{}

bool ModulePlayer::Start() {
	bool ret = true;

	player_x = SCREEN_WIDTH / 2 - SPRITE_SIZE;
	player_y = SCREEN_HEIGHT / 2 - SPRITE_SIZE;

	player_sprite.h = player_sprite.w = SPRITE_SIZE;
	player_sprite.x = 0;
	player_sprite.y = 34;

	player = App->textures->Load("spritesheets/player/spritesheet_player.png");

	return ret;
}

update_status ModulePlayer::Update() {
	App->render->Blit(player, player_x, player_y, 2, &player_sprite);

	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT) {
		if (player_x > 0)
			player_x -= 3;
	}
	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT) {
		if (player_x < SCREEN_WIDTH - SPRITE_SIZE * 2)
			player_x += 3;
	}
	if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT) {
		if (player_y > 0)
			player_y -= 3;
	}
	if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT) {
		if (player_y < SCREEN_HEIGHT - SPRITE_SIZE * 2)
			player_y += 3;
	}
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN) {

		App->particles->AddParticle(App->particles->autoattack, player_x + 25, player_y - 10);
		App->particles->AddParticle(App->particles->autoattack, player_x + 37, player_y - 10);
		//Bullet must travel, for now its static
		//-----------------------
		
	}

	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;
	return ret;
}
