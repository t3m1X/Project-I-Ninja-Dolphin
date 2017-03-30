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
