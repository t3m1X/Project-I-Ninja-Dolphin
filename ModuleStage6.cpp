#include "ModuleStage6.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"

ModuleStage6::ModuleStage6() {
}

ModuleStage6::~ModuleStage6() {
}

bool ModuleStage6::Init() {
	bool ret = true;
	stage_background = App->textures->Load("data/spritesheets/level_backgrounds/tilemap_lvl6.png");
	music = App->audio->LoadMusic("data/music/rough_and_tumble.ogg");
	App->audio->PlayMusic(music);

	SDL_Rect background_rect;
	SDL_QueryTexture(stage_background, nullptr, nullptr, &background_rect.w, &background_rect.h);
	background_pos = -background_rect.h * 2 + SCREEN_HEIGHT; //The multiplier is due to the size of the sprite currently

	return ret;
}

update_status ModuleStage6::Update() {
	App->render->Blit(stage_background, 0, background_pos, 2);
	if (background_pos != 0)
		background_pos += SCROLL_SPEED;

	return UPDATE_CONTINUE;
}

bool ModuleStage6::CleanUp() {
	bool ret = true;
	App->audio->StopMusic();
	return ret;
}
