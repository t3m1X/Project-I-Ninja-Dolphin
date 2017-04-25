#include "ModuleStage1.h"<
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleEnemies.h"
#include "ModuleStageIntro.h"



ModuleStage1::ModuleStage1() {
}

ModuleStage1::~ModuleStage1() {
}

bool ModuleStage1::Start() {
	bool ret = true;
	stage_background = App->textures->Load("revamp_spritesheets/lvl1_placeholder.png");
	music = App->audio->LoadMusic("music/rough_and_tumble.ogg");
	water_texture = App->textures->Load("revamp_spritesheets/base_water_animation.png");


	App->collision->Enable();
    //App->collision->AddCollider({ 320, 120, 50, 60 }, COLLIDER_WALL);

	water.SetUp( 0, 0, 32, 32, 7, 7, "0,1,2,3,4,5,6");
	water.speed = 0.05f;
	water.loop = true;
	
	
	
	App->audio->PlayMusic(music);

	App->player->Enable();
	SDL_Rect background_rect;
	SDL_QueryTexture(stage_background, nullptr, nullptr, &background_rect.w, &background_rect.h);

	//font = App->fonts->LoadFont("fonts/PrStart.ttf", 16);

	
	App->enemies->Enable();

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 5456);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 530, 5000);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 180, 4800);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 210, 4300);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 409, 3400);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 364, 2500);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 370, 5500);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 172, 528);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 570, 528);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 480, 350);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 225, 350);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 480, 5456);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 180, 5456);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 480, 5000);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 180, 4900);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 280, 3400);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 400, 2800);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 365, 800);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 200, 80);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 530, 80);
	
	return ret;
}

update_status ModuleStage1::Update() {
	SDL_Rect background = { 0,0, STAGE_WIDTH, STAGE_HEIGHT };
	App->render->Blit(stage_background, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2, -STAGE_HEIGHT + SCREEN_HEIGHT, &background);
	int y = 0;
	if (!((-STAGE_HEIGHT - 32 * 58) >= App->render->camera.y + SCREEN_HEIGHT)) {
		for (int i = 0; i < 58; i++) {
			for (int j = 0; j < STAGE_WIDTH / 32; ++j) {
				App->render->Blit(water_texture, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2 + j * 32, -STAGE_HEIGHT + SCREEN_HEIGHT + background.h - y, &water.CurrentFrame());
			}
			y += 32;
		}
		water.GetCurrentFrame();
	}

	background.x += background.w;
	App->render->Blit(stage_background, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2 + 4, -STAGE_HEIGHT + SCREEN_HEIGHT -6, &background);
	/*if (-STAGE_HEIGHT + SCREEN_HEIGHT < 0)
		-STAGE_HEIGHT + SCREEN_HEIGHT += SCROLL_SPEED;*/
	if (App->render->camera.y > -STAGE_HEIGHT + SCREEN_HEIGHT)
		App->render->camera.y -= SCROLL_SPEED;

	if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && App->render->camera.x > SCREEN_WIDTH/2 - STAGE_WIDTH/2 )
		App->render->camera.x -= SCROLL_SPEED;

	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && App->render->camera.x < STAGE_WIDTH / 2 - SCREEN_WIDTH / 2 )
		App->render->camera.x += SCROLL_SPEED;

	/*if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT)
		App->transition->Transition(this, App->intro, 0.8f);*/


	
	//App->fonts->WriteText(font, "Test", App->render->camera.x, App->render->camera.y, { 255,255,255,255 });

	return UPDATE_CONTINUE;
}

bool ModuleStage1::CleanUp() {
	bool ret = true;
	App->audio->StopMusic();
	water.CleanUp();
	//App->fonts->EraseFont(font);
	App->enemies->Disable();
	App->collision->Disable();
	App->player->Disable();

	
	return ret;
}
