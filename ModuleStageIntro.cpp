#include "ModuleStageIntro.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"
#include "ModulePlayer.h"
#include "ModuleStage1.h"
#include <stdlib.h>


ModuleStageIntro::ModuleStageIntro() {
}

ModuleStageIntro::~ModuleStageIntro() {
}

bool ModuleStageIntro::Start() {
	bool ret = true;

	srand(SDL_GetTicks());
	music = App->audio->LoadMusic("music/fighting_thunder.ogg");
	title_texture = App->textures->Load("revamp_spritesheets/attract_screen.png");
	stars_texture = App->textures->Load("revamp_spritesheets/attract_screen.png");

	title_screen.SetUp(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 6, 11, "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10");
	title_screen.speed = 0.15f;
	
	stars.SetUp(2396, 820, 204, 204, 5, 5, "0,1,2,3,4");
	stars.speed = 0.05f;
	
	App->audio->PlayMusic(music);
	App->audio->MusicVolume(25);
	App->render->camera = { 0,0 };
	for (int i = 0; i < 5; ++i) {
		stars_array[i].y = 0 + (stars.CurrentFrame().w / 2) * i;
		stars_array[i].x = (rand() % SCREEN_WIDTH) - stars.CurrentFrame().w / 2;
	}

	return ret;
}

update_status ModuleStageIntro::Update() {

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT || App->input->GetControllerButton(1,SDL_CONTROLLER_BUTTON_START) == KEY_REPEAT)
		App->transition->Transition(this, App->stage1, 0.8f);

	App->render->Blit(7, title_texture, 0, 0, { 0,1 }, &title_screen.GetCurrentFrame());
	for (int i = 0; i < 5; ++i) {
		App->render->Blit(6, stars_texture, stars_array[i].x, stars_array[i].y--, { 0,1 }, &stars.GetCurrentFrame());
		if (stars_array[i].y == -stars.CurrentFrame().w)
		{
			stars_array[i].y = SCREEN_HEIGHT;
			stars_array[i].x = (rand() % SCREEN_WIDTH) - stars.CurrentFrame().w / 2;
		}
	}
	//App->render->Blit(6, stars_texture, 0, 0, { 0,1 }, &stars.GetCurrentFrame());
	//App->render->Blit(6, stars_texture, 250, 100, { 0,1 }, &stars.GetCurrentFrame());
	//App->render->Blit(6, stars_texture, 0, 250, { 0,1 }, &stars.GetCurrentFrame());
	//App->render->Blit(6, stars_texture, 250, 300, { 0,1 }, &stars.GetCurrentFrame());

	

	return UPDATE_CONTINUE;
}

bool ModuleStageIntro::CleanUp() {
	bool ret = true;

	if (stars_texture != nullptr) {
		App->textures->Unload(stars_texture);
		stars_texture = nullptr;
	}
	if (title_texture != nullptr) {
		App->textures->Unload(title_texture);
		title_texture = nullptr;
	}

	App->audio->StopMusic();
	if (music != nullptr) {
		App->audio->FreeMusic(music);
		music = nullptr;
	}

	title_screen.CleanUp();
	stars.CleanUp();

	return ret;
}


