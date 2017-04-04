#include "ModuleStage1.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"


ModuleStage1::ModuleStage1() {
}

ModuleStage1::~ModuleStage1() {
}

bool ModuleStage1::Start() {
	bool ret = true;
	stage_background = App->textures->Load("revamp_spritesheets/lvl1_placeholder - copia.png");
	music = App->audio->LoadMusic("music/rough_and_tumble.ogg");
	
	App->audio->PlayMusic(music);

	App->player->Enable();

	SDL_Rect background_rect;
	SDL_QueryTexture(stage_background, nullptr, nullptr, &background_rect.w, &background_rect.h);
	background_pos = -background_rect.h * 2 + SCREEN_HEIGHT; //The multiplier is due to the size of the sprite currently

	return ret;
}

update_status ModuleStage1::Update() {
	App->render->Blit(stage_background, 0, background_pos, 2);
	if (background_pos < 0)
		background_pos += SCROLL_SPEED;

	return UPDATE_CONTINUE;
}

bool ModuleStage1::CleanUp() {
	bool ret = true;
	App->audio->StopMusic();

	App->player->Disable();

	App->stage1->Disable();

	return ret;
}
