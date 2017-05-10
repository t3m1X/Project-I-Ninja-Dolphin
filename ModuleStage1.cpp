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

	App->bonus->Enable();
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

	
	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 200, 6000);
	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 350, 5900);
	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 550, 5800);
     
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 215, 5550);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 5485);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 465, 5550);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_3, 720, 5500);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 350, 5375 + SPAWN_MARGIN);//it's static that why the spawn margin addition
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 200, 5420 + SPAWN_MARGIN);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 500, 5420 + SPAWN_MARGIN);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 350, 5300);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 550, 5200);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 200, 5300);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 470, 4350);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 270, 5200);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 400, 5200);
	App->enemies->AddEnemy(ENEMY_TYPES::BOMB, 350, 5200);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 215, 4950);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 350, 4900);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 465, 4950);

	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 350, 4800);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 160, 4600);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 290, 4600);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 420, 4600);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 550, 4600);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 25, 4500);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 470, 4350);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 200, 4350);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 350, 4280);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 4100);
	App->enemies->AddEnemy(ENEMY_TYPES::BOMB, 250, 4300);
	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 220, 4200);
	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 500, 4250);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 25, 3800);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 220, 3800);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 470, 3800);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 3720);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 190, 3500);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 500, 3500);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 275, 3450);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 455, 3450);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 3400);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 245, 3100);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 450, 3100);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 350, 3100);
	App->enemies->AddEnemy(ENEMY_TYPES::BOMB, 450, 3150);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 25, 3000);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 260, 2780);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 2850);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 470, 2780);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 25, 2400);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 170, 2440);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 470, 2370);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 470, 2370);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 500, 2440);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 2000);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 220, 1900);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 500, 1900);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 190, 1450);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 290, 1300);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 1370);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 420, 1300);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 500, 1450);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 25, 1100);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 150, 870);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 780);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 600, 780);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 210, 300);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 520, 300);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 466, 160);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 400);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 284, 160);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 195, 260);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 530, 260);
	/*App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 5456);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 400, 5000);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 180, 4800);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 210, 4300);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 409, 3400);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 364, 2500);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 370, 5500);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 172, 528);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 570, 528);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 480, 530);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 225, 460);

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 480, 5456);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 180, 5456);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 480, 5000);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 180, 4900);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 280, 3400);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 400, 2800);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 365, 800);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 200, 500);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 530, 600);
	*/
	
	//App->bonus->AddBonus(BONUS_TYPE::RED, 220, 5540);

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
	water.CleanUp();
	//App->fonts->EraseFont(font);
	App->enemies->Disable();
	App->collision->Disable();
	App->player->Disable();
	App->bonus->Disable();

	
	return ret;
}
