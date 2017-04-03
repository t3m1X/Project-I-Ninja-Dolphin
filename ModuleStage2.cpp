#include "ModuleStage2.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleStageIntro.h"

ModuleStage2::ModuleStage2() {
}

ModuleStage2::~ModuleStage2() {
}

bool ModuleStage2::Start() {
	bool ret = true;
	stage_background = App->textures->Load("spritesheets/level_backgrounds/tilemap_lvl2.png");
	music = App->audio->LoadMusic("music/rough_and_tumble.ogg");
	boss_music = App->audio->LoadMusic("music/go_to_blazes.ogg");
	App->audio->PlayMusic(music);

	SDL_Rect background_rect;
	SDL_QueryTexture(stage_background, nullptr, nullptr, &background_rect.w, &background_rect.h);
	background_pos = -background_rect.h * 2 + SCREEN_HEIGHT; //The multiplier is due to the size of the sprite currently

	return ret;
}

update_status ModuleStage2::Update() {
	App->render->Blit(stage_background, 0, background_pos, 2);
	if (background_pos < 0)
		background_pos += SCROLL_SPEED;

	return UPDATE_CONTINUE;
}

bool ModuleStage2::CleanUp() {
	bool ret = true;
	App->audio->StopMusic();

	App->stage2->Disable();

	return ret;
}
