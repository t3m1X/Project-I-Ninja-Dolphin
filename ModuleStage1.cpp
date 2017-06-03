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
	background_animations = App->textures->Load("revamp_spritesheets/backgroundanimations.png");

	App->bonus->Enable();
	App->collision->Enable();
   
	sea_water.SetUp( 0, 0, 32, 32, 7, 7, "0,1,2,3,4,5,6");
	sea_water.speed = 0.05f;
	sea_water.loop = true;
	river_water.SetUp(0, 32, 32, 32, 7, 7, "0,1,2,3,4,5,6");
	river_water.speed = 0.05f;
	river_water.loop = true;

	coast.SetUp(0, 0, 704, 93, 1, 7, "0,1,2,3,4,5,6");
	coast.speed = 0.05f;
	coast.loop = true;
	
	cloud_position = SCREEN_HEIGHT - STAGE_HEIGHT;
	
	App->audio->PlayMusic(music);
	App->audio->MusicVolume(10);

	App->player->Enable();
	SDL_Rect background_rect;
	SDL_QueryTexture(stage_background, nullptr, nullptr, &background_rect.w, &background_rect.h);

	//font = App->fonts->LoadFont("fonts/PrStart.ttf", 16);

	
	App->enemies->Enable();

	
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 300, 5600);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 250, 5520);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTAIRSHIP, 350, 5500);
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

	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 350, 5427);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 420, 3900);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 400, 2888);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 443, 1711);
	App->enemies->AddEnemy(ENEMY_TYPES::BONUSAIRSHIP, 331, 920);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 397, 2452);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 507, 2414);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 253, 2423);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK, 92, 2336);

	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 180, 5160);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 365, 5020);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 240, 4167);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 145, 3353);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 517, 2455);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 169, 2353);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 621, 2166);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 495, 2161);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_2, 178, 2200);

	
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_3, 496, 5400);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_3, 80, 4500);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_3, 655, 4445);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_3, 667, 3731);
	App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_3, 676, 3698);
	/*App->enemies->AddEnemy(ENEMY_TYPES::LIGHTTANK_3, 231, 3479);*/

	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 226, 4979);
	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 378, 4669);
	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 477, 4669);
	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 378, 4605);
	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 477, 4605);
	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 603, 4171);
	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 133, 4034);
	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 199, 2680);
	App->enemies->AddEnemy(ENEMY_TYPES::BOX, 556, 2631);

	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 315, 4842);
	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 464, 4640);
	App->enemies->AddEnemy(ENEMY_TYPES::KAMIKAZE, 317, 4590);

	App->enemies->AddEnemy(ENEMY_TYPES::ROTATORYTANK, 453, 3609);
	App->enemies->AddEnemy(ENEMY_TYPES::ROTATORYTANK, 121, 3609);

	App->enemies->AddEnemy(ENEMY_TYPES::BOMB, 375, 1037);

	App->enemies->AddEnemy(ENEMY_TYPES::TURRET, 214, 2900);
	App->enemies->AddEnemy(ENEMY_TYPES::TURRET, 358, 2828);
	App->enemies->AddEnemy(ENEMY_TYPES::TURRET, 482, 2765);
	App->enemies->AddEnemy(ENEMY_TYPES::TURRET, 608, 2708);

	App->enemies->AddEnemy(ENEMY_TYPES::MOONAIRSHIP, 140, 2290);
	App->enemies->AddEnemy(ENEMY_TYPES::MOONAIRSHIP, 520, 2290);

	App->enemies->AddEnemy(ENEMY_TYPES::BOSS, 290, 700);
	App->enemies->AddEnemy(ENEMY_TYPES::BOSS_2, 290, 30);

	return ret;
}

update_status ModuleStage1::Update() {

	SDL_Rect background = { 0,0, STAGE_WIDTH, STAGE_HEIGHT };
	App->render->Blit(1, stage_background, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2, -STAGE_HEIGHT + SCREEN_HEIGHT, { 0,1 }, &background);
	int y = 0;

	/*if (!((-STAGE_HEIGHT - 2000) >= App->render->camera.y + SCREEN_HEIGHT)) 
		App->render->Blit(0, shore_texture, 0, 5700, { 0,1 }, &shore.CurrentFrame());*/
	

	background.x += background.w;
	App->render->Blit(4, stage_background, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2, -STAGE_HEIGHT + SCREEN_HEIGHT - 6, { 0,1 }, &background);
	/*if (-STAGE_HEIGHT + SCREEN_HEIGHT < 0)
		-STAGE_HEIGHT + SCREEN_HEIGHT += SCROLL_SPEED;*/
	if (App->render->camera.y > -STAGE_HEIGHT + SCREEN_HEIGHT) {
		App->render->camera.y -= SCROLL_SPEED;
	}
	/*else
	{
		App->player->Disable();
		App->transition->Transition(App->stage1, App->intro, 0.8f);
	}
*/

	App->render->Blit(7, background_animations, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2, cloud_position, { 0,1 }, &background);
	App->render->Blit(7, background_animations, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2, cloud_position - STAGE_HEIGHT, { 0,1 }, &background);
	/*if (App->render->camera.y % 2)*/
	cloud_position += 1;

	if (cloud_position >= 0)
		cloud_position = SCREEN_HEIGHT - STAGE_HEIGHT;

	PrintWater(&sea_water, 7253, 58);
	PrintWater(&river_water, 5161, 14);
	PrintWater(&river_water, 3205, 15);

	App->render->Blit(1, background_animations, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2 , SCREEN_HEIGHT - STAGE_HEIGHT + 5758, { 0,1 }, &coast.GetCurrentFrame());

	/*if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT)
		App->transition->Transition(this, App->intro, 0.8f);*/

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
	sea_water.CleanUp();
	river_water.CleanUp();
	coast.CleanUp();
	App->enemies->Disable();
	App->collision->Disable();
	App->player->Disable();
	App->bonus->Disable();

	
	return ret;
}

void ModuleStage1::PrintWater(Animation * anim, int y_start, int n_tiles)
{
	y_start -= STAGE_HEIGHT - SCREEN_HEIGHT;
	for (int i = 1; i <= n_tiles; i++) {
		if (App->render->camera.y - SPAWN_MARGIN <= y_start - 32 * i && App->render->camera.y + SCREEN_HEIGHT > y_start - 32 * i)
			for (int j = 0; j < STAGE_WIDTH / 32; ++j)
				App->render->Blit(0, water_texture, SCREEN_WIDTH / 2 - STAGE_WIDTH / 2 + j * 32, y_start - 32 * i, { 0,1 }, &anim->CurrentFrame());
		
	}
	anim->GetCurrentFrame();
}
