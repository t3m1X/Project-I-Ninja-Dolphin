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
	stage_background = App->textures->Load("revamp_spritesheets/lvl1_placeholder.png");
	music = App->audio->LoadMusic("music/rough_and_tumble.ogg");
	water_texture = App->textures->Load("revamp_spritesheets/base_water_animation.png");

	water.SetUp( 0, 0, 32, 32, 2, 2, "0,0,1,0");
	water.speed = 0.04f;
	water.loop = true;
	
	
	
	App->audio->PlayMusic(music);

	App->player->Enable();

	SDL_Rect background_rect;
	SDL_QueryTexture(stage_background, nullptr, nullptr, &background_rect.w, &background_rect.h);
	background_pos = -background_rect.h  + SCREEN_HEIGHT; //The multiplier is due to the size of the sprite currently
	return ret;
}

update_status ModuleStage1::Update() {
	SDL_Rect background = { 0,0,704,6532 };
	App->render->Blit(stage_background, 0, background_pos, 1, &background);
	background.x += background.w;
	App->render->Blit(stage_background, 0, background_pos, 1, &background);
	if (background_pos < 0)
		background_pos += SCROLL_SPEED;

	App->render->Blit(water_texture, 0, 0, 1, &water.GetCurrentFrame());
	

	return UPDATE_CONTINUE;
}

bool ModuleStage1::CleanUp() {
	bool ret = true;
	App->audio->StopMusic();
	water.CleanUp();
	App->player->Disable();
	App->stage1->Disable();

	return ret;
}
