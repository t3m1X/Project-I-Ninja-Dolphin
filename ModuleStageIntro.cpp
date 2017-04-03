#include "ModuleStageIntro.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleStage2.h"


ModuleStageIntro::ModuleStageIntro() {
}

ModuleStageIntro::~ModuleStageIntro() {
}

bool ModuleStageIntro::Start() {
	bool ret = true;

	music = App->audio->LoadMusic("music/fighting_thunder.ogg");
	title_texture = App->textures->Load("revamp_spritesheets/attract_screen.png");

	title_screen.SetUp(0, 0, 518, 592, 6, 11, "0,0,0,0,0,1,2,3,4,5,6,7,8,9,10");
	title_screen.speed = 0.1;
	
	App->audio->PlayMusic(music);

	return ret;
}

update_status ModuleStageIntro::Update() {

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT)
		App->fade->FadeToBlack(this, App->stage2, 2.0f);

	App->render->Blit(title_texture, 0, 0, 1, &title_screen.GetCurrentFrame());

	return UPDATE_CONTINUE;
}

bool ModuleStageIntro::CleanUp() {
	bool ret = true;
	App->audio->StopMusic();
	//
	App->stage6->Disable();
	//

	return ret;
}
