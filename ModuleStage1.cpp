#include "ModuleStage1.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleEnemies.h"
#include "ModuleStageIntro.h"
#include "ModuleBonus.h"



ModuleStage1::ModuleStage1() {
}

ModuleStage1::~ModuleStage1() {
}

bool ModuleStage1::Start() {
	bool ret = true;
	stage_background = App->textures->Load("revamp_spritesheets/lvl1_placeholder.png");
	music = App->audio->LoadMusic("music/rough_and_tumble.ogg");
	water_texture = App->textures->Load("revamp_spritesheets/base_water_animation.png");
	shore_texture = App->textures->Load("revamp_spritesheets/backgroundanimations.png");

	App->bonus->Enable();
	App->collision->Enable();
    //App->collision->AddCollider({ 320, 120, 50, 60 }, COLLIDER_WALL);

	water.SetUp( 0, 0, 32, 32, 7, 7, "0,1,2,3,4,5,6");
	water.speed = 0.05f;
	water.loop = true;
	
	shore.SetUp(0, 25, 703, 100, 1, 7, "0,1,2,3,4,5,6");
	shore.speed = 0.05f;
	shore.loop = true;

	
	
	App->audio->PlayMusic(music);

	App->player->Enable();
	SDL_Rect background_rect;
	SDL_QueryTexture(stage_background, nullptr, nullptr, &background_rect.w, &background_rect.h);

	//font = App->fonts->LoadFont("fonts/PrStart.ttf", 16);

	
	App->enemies->Enable();

	App->enemies->AddEnemy(ENEMY_TYPES::ROTATORYTANK, 150, 6300);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 300, 5900);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 250, 5820);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 5800);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 340, 4360);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 550, 4300);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 420, 4260);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 460, 4225);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 320, 4175);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 435, 4100);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 460, 3420);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 175, 3330);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 510, 3290);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 425, 3235);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 570, 3190);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 480, 3175);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 504, 3130);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 450, 3115);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 520, 3088);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 465, 3050);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 540, 3040);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 455, 2987);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 520, 2973);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 563, 2935);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 195, 2455);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 390, 2377);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 490, 2345);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 540, 2315);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 485, 2290);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 417, 1756);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 555, 1691);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 459, 1691);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 400, 1636);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 550, 1570);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 355, 1565);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 443, 1541);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 447, 1502);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 369, 1477);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 499, 1440);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 1436);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 395, 1421);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 447, 1402);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 411, 1362);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 500, 1342);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 467, 1316);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 495, 1292);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 385, 913);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 235, 849);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 345, 825);

	
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 350, 5375 + SPAWN_MARGIN);//it's static that why the spawn margin addition
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 200, 5420 + SPAWN_MARGIN);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 500, 5420 + SPAWN_MARGIN);
	

	
	

	

	
	
	

	

	return ret;
}

update_status ModuleStage1::Update() {

	if (App->input->HasController(1)) {
		float axis_x = App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX);
		if (axis_x < 0 && App->render->camera.x > SCREEN_WIDTH / 2 - STAGE_WIDTH / 2) {
			App->render->camera.x += (SCROLL_SPEED + 1) * axis_x;
			if (App->render->camera.x < SCREEN_WIDTH / 2 - STAGE_WIDTH / 2)
				App->render->camera.x = SCREEN_WIDTH / 2 - STAGE_WIDTH / 2;
		}

		if (axis_x > 0 && App->render->camera.x < STAGE_WIDTH / 2 - SCREEN_WIDTH / 2) {
			App->render->camera.x += (SCROLL_SPEED + 1) * axis_x;
			if (App->render->camera.x > STAGE_WIDTH / 2 - SCREEN_WIDTH / 2)
				App->render->camera.x = STAGE_WIDTH / 2 - SCREEN_WIDTH / 2;
		}
	}
	else {
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && App->render->camera.x > SCREEN_WIDTH / 2 - STAGE_WIDTH / 2)
			App->render->camera.x -= SCROLL_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && App->render->camera.x < STAGE_WIDTH / 2 - SCREEN_WIDTH / 2)
			App->render->camera.x += SCROLL_SPEED;
	}

	SDL_Rect background = { 0,0, STAGE_WIDTH, STAGE_HEIGHT };
	App->render->Blit(1, stage_background, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2, -STAGE_HEIGHT + SCREEN_HEIGHT, { 0,1 }, &background);
	int y = 0;
	if (!((-STAGE_HEIGHT - 32 * 58) >= App->render->camera.y + SCREEN_HEIGHT)) {
		for (int i = 0; i < 58; i++) {
			for (int j = 0; j < STAGE_WIDTH / 32; ++j) {
				App->render->Blit(0, water_texture, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2 + j * 32, -STAGE_HEIGHT + SCREEN_HEIGHT + background.h - y, { 0,1 }, &water.CurrentFrame());
			}
			y += 32;
		}
		water.GetCurrentFrame();
	}

	/*if (!((-STAGE_HEIGHT - 2000) >= App->render->camera.y + SCREEN_HEIGHT)) 
		App->render->Blit(0, shore_texture, 0, 5700, { 0,1 }, &shore.CurrentFrame());*/
	

	background.x += background.w;
	App->render->Blit(4, stage_background, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2 + 4, -STAGE_HEIGHT + SCREEN_HEIGHT - 6, { 0,1 }, &background);
	/*if (-STAGE_HEIGHT + SCREEN_HEIGHT < 0)
		-STAGE_HEIGHT + SCREEN_HEIGHT += SCROLL_SPEED;*/
	if (App->render->camera.y > -STAGE_HEIGHT + SCREEN_HEIGHT) {
		App->render->camera.y -= SCROLL_SPEED;
		App->player->AddScore(1);
	}
	else
	{
		App->player->Disable();
		App->transition->Transition(App->stage1, App->intro, 0.8f);
	}

	
	/*if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT)
		App->transition->Transition(this, App->intro, 0.8f);*/


	
	//App->fonts->WriteText(font, "Test", App->render->camera.x, App->render->camera.y, { 255,255,255,255 });

	return UPDATE_CONTINUE;
}

bool ModuleStage1::CleanUp() {
	bool ret = true;
	if(shore_texture != nullptr) {
		App->textures->Unload(shore_texture);
		shore_texture = nullptr;
	}
	if (water_texture != nullptr) {
		App->textures->Unload(water_texture);
		water_texture = nullptr;
	}
	if (stage_background != nullptr) {
		App->textures->Unload(stage_background);
		stage_background = nullptr;
	}
	App->audio->StopMusic();
	if (music != nullptr) {
		App->audio->FreeMusic(music);
		music = nullptr;
	}
	shore.CleanUp();
	water.CleanUp();
	//App->fonts->EraseFont(font);
	App->enemies->Disable();
	App->collision->Disable();
	App->player->Disable();
	App->bonus->Disable();

	
	return ret;
}
