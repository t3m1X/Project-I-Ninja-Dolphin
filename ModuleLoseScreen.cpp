#include "ModuleLoseScreen.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"
#include "ModulePlayer.h"
#include "ModuleStage1.h"


ModuleLoseScreen::ModuleLoseScreen() {
}

ModuleLoseScreen::~ModuleLoseScreen() {
}

bool ModuleLoseScreen::Start() {
	bool ret = true;

	music = App->audio->LoadMusic("music/fighting_thunder.ogg");
	title_texture = App->textures->Load("revamp_spritesheets/lose_screen.png");
	stars_texture = App->textures->Load("revamp_spritesheets/attract_screen.png");

	title_screen.SetUp(0, 0, 239, 151, 3, 5, "0,1,2,3,4");
	title_screen.speed = 0.1f;

	stars.SetUp(2396, 820, 204, 204, 5, 5, "0,1,2,3,4");
	stars.speed = 0.05f;

	App->audio->PlayMusic(music);
	App->audio->MusicVolume(25);
	App->render->camera = { 0,0 };



	return ret;
}

update_status ModuleLoseScreen::Update() {

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT || App->input->GetControllerButton(1, SDL_CONTROLLER_BUTTON_START) == KEY_REPEAT)
		App->transition->Transition(this, App->stage1, 0.8f);

	App->render->Blit(7, title_texture, 105, 170, { 0,1 }, &title_screen.GetCurrentFrame());
	App->render->Blit(6, stars_texture, 0, 0, { 0,1 }, &stars.GetCurrentFrame());
	App->render->Blit(6, stars_texture, 250, 100, { 0,1 }, &stars.GetCurrentFrame());
	App->render->Blit(6, stars_texture, 0, 250, { 0,1 }, &stars.GetCurrentFrame());
	App->render->Blit(6, stars_texture, 250, 300, { 0,1 }, &stars.GetCurrentFrame());



	return UPDATE_CONTINUE;
}

update_status ModuleLoseScreen::PostUpdate()
{
	DirectLose();

	return UPDATE_CONTINUE;
}

void ModuleLoseScreen::DirectLose() {

	if (App->input->keyboard[SDL_SCANCODE_F12] == KEY_DOWN)
	{
		App->player->Disable();
		App->transition->Transition(App->stage1, App->losescreen, 0.8f);
		loosingscreen = !loosingscreen;
	}

	if (!loosingscreen)
		return;
}

bool ModuleLoseScreen::CleanUp() {
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
