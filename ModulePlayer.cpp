#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleTransition.h"
#include "ModuleStageIntro.h"
#include "ModuleStage1.h"


ModulePlayer::ModulePlayer() 
{}

ModulePlayer::~ModulePlayer() 
{}

bool ModulePlayer::Start() {
	bool ret = true;

	score_text = new char[16];
	char* tmp = "SCORE:        0";
	for (int i = 0; i <= 15; i++, ++tmp)
		score_text[i] = *tmp;
	highscore_text = new char[20];
	char* tmp2 = "HIGHSCORE:        0";
	for (int i = 0; i <= 19; i++, ++tmp2)
		highscore_text[i] = *tmp2;

	score = 0;

	player_x = SCREEN_WIDTH / 2 - SPRITE_SIZE;
	player_y = SCREEN_HEIGHT / 2 - SPRITE_SIZE;

	player_sprite.h = SPRITE_HEIGHT;
	player_sprite.w = SPRITE_WIDTH;
	player_sprite.x = 0;
	player_sprite.y = 0;

	player_sprite_left.h = SPRITE_HEIGHT;
	player_sprite_left.w = SPRITE_WIDTH;
	player_sprite_left.x = SPRITE_WIDTH;
	player_sprite_left.y = 0;

	player_sprite_right.h = SPRITE_HEIGHT;
	player_sprite_right.w = SPRITE_WIDTH;
	player_sprite_right.x = SPRITE_WIDTH * 2;
	player_sprite_right.y = 0;

	player_sprite_godmode.h = SPRITE_HEIGHT;
	player_sprite_godmode.w = SPRITE_WIDTH + 2;
	player_sprite_godmode.x = 262;
	player_sprite_godmode.y = 0;

	player_sprite_godmode_left.h = SPRITE_HEIGHT;
	player_sprite_godmode_left.w = SPRITE_WIDTH;
	player_sprite_godmode_left.x = 262 + SPRITE_WIDTH + 2;
	player_sprite_godmode_left.y = 0;

	player_sprite_godmode_right.h = SPRITE_HEIGHT;
	player_sprite_godmode_right.w = SPRITE_WIDTH;
	player_sprite_godmode_right.x = 262 + SPRITE_WIDTH * 2 + 2;
	player_sprite_godmode_right.y = 0;

	shadow_idle.h = SHADOW_HEIGHT;
	shadow_idle.w = SHADOW_WIDTH;
	shadow_idle.x = 171;
	shadow_idle.y = 0;

	shadow_left.h = SHADOW_HEIGHT;
	shadow_left.w = SHADOW_WIDTH;
	shadow_left.x = 171 + SHADOW_WIDTH;
	shadow_left.y = 0;

	shadow_right.h = SHADOW_HEIGHT;
	shadow_right.w = SHADOW_WIDTH;
	shadow_right.x = 171 + SHADOW_WIDTH * 2;
	shadow_right.y = 0;


	player = App->textures->Load("revamp_spritesheets/player_spritesheet.png");
	laser_sfx = App->audio->LoadSFX("sfx/shot_regular.wav");
	font = App->fonts->LoadFont("fonts/PrStart.ttf", 8);

	sdl_shot = 0;

	state = IDLE;

	
	player_collider = App->collision->AddCollider({0, 0, 60, 50}, COLLIDER_PLAYER, this);

	player_fire_forward.SetUp(0, 49, 57, 66, 5, 5, "0,1,2,3,4");
	player_fire_forward.speed = 0.2f;
	player_fire_left.SetUp(0, 115, 57, 66, 5, 5, "0,1,2,3,4");
	player_fire_left.speed = 0.2f;
	player_fire_right.SetUp(0, 181, 57, 66, 5, 5, "0,1,2,3,4");
	player_fire_right.speed = 0.2f;

	if (highscore != 0) {
		uint tmp = highscore;
		int i = 19 - 1;
		while (tmp != 0) {
			highscore_text[i] = '0' + tmp % 10;
			tmp /= 10;
			--i;
		}
	}


	return ret;
}

update_status ModulePlayer::Update() {

	sdl_clock = SDL_GetTicks();

	switch (state) {
	case IDLE:
		if (godmode)
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0, 1 }, &player_sprite_godmode);

		else
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0, 1 }, &player_sprite);

		App->render->Blit(player, App->render->camera.x + player_x + 47, App->render->camera.y + player_y + 38, { 0,1 }, &shadow_idle);
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_forward.GetCurrentFrame());
		
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT))
			state = LEFT;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT))
			state = RIGHT;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT))
			state = FORWARD;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT))
			state = STOP;
			

		break;

	case LEFT:
		if (godmode)
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite_godmode_left);
		else
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite_left);
		
		App->render->Blit(player, App->render->camera.x + player_x + 47, App->render->camera.y + player_y + 38, { 0,1 }, &shadow_left);
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_left.GetCurrentFrame());


		if (player_x > -SPRITE_WIDTH / 2)
			player_x -= PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_UP || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT)
			state = IDLE;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
			&& player_y > SPRITE_HEIGHT)
			player_y -= PLAYER_SPEED;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
			&& player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
			player_y += PLAYER_SPEED;
		break;

	case RIGHT:

		if (godmode)
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite_godmode_right);
		else
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite_right);
		
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_right.GetCurrentFrame());
		App->render->Blit(player, App->render->camera.x + player_x + 47, App->render->camera.y + player_y + 38, { 0,1 }, &shadow_right);


		if (player_x < SCREEN_WIDTH - SPRITE_WIDTH / 2)
			player_x += PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_UP || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT)
			state = IDLE;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
			&& player_y > SPRITE_HEIGHT)
			player_y -= PLAYER_SPEED;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
			&& player_y  < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
			player_y += PLAYER_SPEED;
		break;

	case FORWARD:

		if (godmode)
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite_godmode);
		else
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite);
		
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_forward.GetCurrentFrame());
		App->render->Blit(player, App->render->camera.x + player_x + 47, App->render->camera.y + player_y + 38, { 0,1 }, &shadow_idle);

		if (player_y > SPRITE_HEIGHT)
			player_y -= PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_UP || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
			state = IDLE;
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT))
			state = LEFT;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT))
			state = RIGHT;
		break;

	case STOP:

		if (godmode)
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite_godmode);
		else
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite);

		App->render->Blit(player, App->render->camera.x + player_x + 47, App->render->camera.y + player_y + 38, { 0,1 }, &shadow_idle);

		

		if (player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
			player_y += PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_UP || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
			state = IDLE;
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT))
			state = LEFT;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT))
			state = RIGHT;
		break;
	}
	
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT && sdl_clock > sdl_shot) {
		
		sdl_shot = sdl_clock + LASER_COOLDOWN; 
		App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 18, App->render->camera.y + player_y + 16);
		App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 35, App->render->camera.y + player_y + 16);
		App->audio->PlaySFX(laser_sfx);
		
	}

	if (App->input->keyboard[SDL_SCANCODE_F2] == KEY_DOWN)
	{
		godmode = !godmode;
	}


	player_collider->SetPos(App->render->camera.x + player_x, App->render->camera.y + player_y);

	if (score != 0) {
		uint tmp = score;
		int i = 15 - 1;
		while (tmp != 0) {
			score_text[i] = '0' + tmp % 10;
			tmp /= 10;
			--i;
		}
	}	

	App->fonts->WriteText(font, score_text, App->render->camera.x +5, App->render->camera.y + 8, { 0,0,0});
	App->fonts->WriteText(font, score_text, App->render->camera.x + 5, App->render->camera.y + 5, { 255,255,255 });
	App->fonts->WriteText(font, highscore_text, App->render->camera.x + 135, App->render->camera.y + 8, { 0,0,0 });
	App->fonts->WriteText(font, highscore_text, App->render->camera.x + 135, App->render->camera.y + 5, { 255,255,255 });
	

	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;

	if (player_collider != nullptr)
		player_collider->to_delete = true;

	player_fire_forward.CleanUp();
	player_fire_left.CleanUp();
	player_fire_right.CleanUp();
	App->textures->Unload(player);
	if (laser_sfx != nullptr) {
		App->audio->FreeSFX(laser_sfx);
		laser_sfx = nullptr;
	}
	if (font != nullptr) {
		App->fonts->EraseFont(font);
		font = nullptr;
	}
	if (score > highscore)
		highscore = score;
	if (highscore_text != nullptr) {
		delete[] highscore_text;
		highscore_text = nullptr;
	}
	if (score_text != nullptr) {
		delete[] score_text;
		score_text = nullptr;
	}

	return ret;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{

	if (godmode == true)
	{
		return;
	}

	else
	{
		if (c1->type == COLLIDER_ENEMY_AIR || c2->type == COLLIDER_ENEMY_AIR ||
			c1->type == COLLIDER_ENEMY_SHOT || c2->type == COLLIDER_ENEMY_SHOT)
		{
			App->particles->AddParticle(EXPLOSION, App->render->camera.x + player_x, App->render->camera.y + player_y);
			App->player->Disable();
			App->transition->Transition(App->stage1, App->intro, 0.8f);
		}

	}
}

iPoint ModulePlayer::GetPos()
{
	return { App->render->camera.x + player_x, App->render->camera.y + player_y };
}

void ModulePlayer::AddScore(uint score_add)
{
	score += score_add;
}
