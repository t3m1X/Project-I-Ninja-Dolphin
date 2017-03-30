#include "ModuleStage6.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleStage2.h"


ModuleStage6::ModuleStage6() {
}

ModuleStage6::~ModuleStage6() {
}

bool ModuleStage6::Start() {
	bool ret = true;

	stage_background = App->textures->Load("spritesheets/level_backgrounds/tilemap_lvl6.png");
	music = App->audio->LoadMusic("music/rough_and_tumble.ogg");
	boss_music = App->audio->LoadMusic("music/go_to_blazes.ogg");
	App->audio->PlayMusic(music);

	SDL_Rect background_rect;
	SDL_QueryTexture(stage_background, nullptr, nullptr, &background_rect.w, &background_rect.h);
	background_pos = -background_rect.h * 2 + SCREEN_HEIGHT; //The multiplier is due to the size of the sprite currently

															 //#### This part is to be done in a new module (Player)
	player_x = SCREEN_WIDTH / 2 - SPRITE_SIZE;
	player_y = SCREEN_HEIGHT / 2 - SPRITE_SIZE;

	player_sprite.h = player_sprite.w = SPRITE_SIZE;
	player_sprite.x = 0;
	player_sprite.y = 34;

	player = App->textures->Load("spritesheets/player/spritesheet_player.png");
	//####

	return ret;
}

update_status ModuleStage6::Update() {
	App->render->Blit(stage_background, 0, background_pos, 2);
	if (background_pos < 0)
		background_pos += SCROLL_SPEED;
	else
		App->fade->FadeToBlack(this, App->stage2, 2.0f);

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT)
		App->fade->FadeToBlack(this, App->stage2, 2.0f);

	//#### This part is to be done in a new module (Player)
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
	//####

	return UPDATE_CONTINUE;
}

bool ModuleStage6::CleanUp() {
	bool ret = true;
	App->audio->StopMusic();
	//
	App->stage6->Disable();
	//

	return ret;
}
