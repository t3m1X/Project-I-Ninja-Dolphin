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
#include "ModuleBonus.h"


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

	player_x = SCREEN_WIDTH / 2 - SPRITE_WIDTH / 2;
	player_y = SCREEN_HEIGHT / 2 - SPRITE_HEIGHT / 2;

	player_sprite.h = SPRITE_HEIGHT;
	player_sprite.w = SPRITE_WIDTH;
	player_sprite.x = 0;
	player_sprite.y = 0;

	player_sprite_godmode.h = SPRITE_HEIGHT + 1;
	player_sprite_godmode.w = SPRITE_WIDTH + 2;
	player_sprite_godmode.x = 0;
	player_sprite_godmode.y = 416;

	shadow_idle.h = SHADOW_HEIGHT;
	shadow_idle.w = SHADOW_WIDTH;
	shadow_idle.x = 285;
	shadow_idle.y = 0;

	shadow_left.h = SHADOW_HEIGHT;
	shadow_left.w = SHADOW_WIDTH;
	shadow_left.x = 285 + SHADOW_WIDTH;
	shadow_left.y = 0;

	shadow_right.h = SHADOW_HEIGHT;
	shadow_right.w = SHADOW_WIDTH;
	shadow_right.x = 285 + SHADOW_WIDTH * 2;
	shadow_right.y = 0;


	player = App->textures->Load("revamp_spritesheets/player_spritesheet.png");
	laser_sfx = App->audio->LoadSFX("sfx/shot_regular.wav");
	font = App->fonts->LoadFont("fonts/PrStart.ttf", 8);

	sdl_shot = 0;

	state = IDLE;

	
	player_collider = App->collision->AddCollider({0, 0, 60, 50}, COLLIDER_PLAYER, this);

	player_fire_forward.SetUp(0, 49, SPRITE_WIDTH, SPRITE_HEIGHT + 19, 5, 5, "0,1,2,3,4");
	player_fire_forward.speed = 0.2f;

	player_fire_left.SetUp(0, 117, SPRITE_WIDTH, SPRITE_HEIGHT + 19, 5, 10, "0,1,2,3,5,6,7,8,9");
	player_fire_left.LoopStart(4);
	player_fire_left.speed = 0.2f;

	player_fire_right.SetUp(0, 185, SPRITE_WIDTH, SPRITE_HEIGHT + 19, 5, 10, "0,1,2,3,5,6,7,8,9");
	player_fire_right.LoopStart(4);
	player_fire_right.speed = 0.2f;

	player_left.SetUp( 57, 0, SPRITE_WIDTH, SPRITE_HEIGHT, 2, 2, "0,1");
	player_left.speed = 0.05f;
	player_left.loop = false;

	player_right.SetUp(171, 0, SPRITE_WIDTH, SPRITE_HEIGHT, 2, 2, "0,1");
	player_right.speed = 0.05f;
	player_right.loop = false;

	player_left_godmode.SetUp(61, 416, SPRITE_WIDTH, SPRITE_HEIGHT + 1, 2, 2, "0,1");
	player_left_godmode.speed = 0.05f;
	player_left_godmode.loop = false;

	player_right_godmode.SetUp(177, 416, SPRITE_WIDTH, SPRITE_HEIGHT + 1, 2, 2, "0,1");
	player_right_godmode.speed = 0.05f;
	player_right_godmode.loop = false;

	if (highscore != 0) {
		uint tmp = highscore;
		int i = 19 - 1;
		while (tmp != 0) {
			highscore_text[i] = '0' + tmp % 10;
			tmp /= 10;
			--i;
		}
	}

	current_bonus = RED_BONUS;
	amount_bonus = 0;

	return ret;
}

update_status ModulePlayer::Update() {

	sdl_clock = SDL_GetTicks();

	switch (state) {
	case IDLE:
		if (godmode)
			App->render->Blit(6, player, App->render->camera.x + player_x - 1, App->render->camera.y + player_y - 1, { 0, 1 }, &player_sprite_godmode);

		else
			App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0, 1 }, &player_sprite);

		App->render->Blit(5, player, App->render->camera.x + player_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_idle);
		App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_forward.GetCurrentFrame());
		
		if (App->input->HasController(1)) {
			if (App->input->GetControllerAxis(1,SDL_CONTROLLER_AXIS_LEFTX) < -0.3)
				state = LEFT;
			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX) > 0.3)
				state = RIGHT;
			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY) < -0.3)
				state = FORWARD;
			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY) > 0.3)
				state = STOP;
		}
		else {
			if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT))
				state = LEFT;
			if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT))
				state = RIGHT;
			if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT))
				state = FORWARD;
			if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT))
				state = STOP;
		}
			

		break;

	case LEFT:
		if (godmode)
			App->render->Blit(6, player, App->render->camera.x + player_x - 1, App->render->camera.y + player_y - 1, { 0,1 }, &player_left_godmode.GetCurrentFrame());
	
		else
			App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_left.GetCurrentFrame());
			
		App->render->Blit(5, player, App->render->camera.x + player_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_left);
		
		if (App->input->HasController(1)) {
			if (player_x > -SPRITE_WIDTH / 2)
				player_x += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(1,SDL_CONTROLLER_AXIS_LEFTX);

			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX) >= -0.3) {
				state = IDLE;

				player_left_godmode.Reset();
				player_left.Reset();

			}

			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY) > 0.3)
				player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY);
			else {
				App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_left.GetCurrentFrame());
				if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY) < -0.3)
					player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY);
			}

			if (player_y <= SPRITE_HEIGHT)
				player_y = SPRITE_HEIGHT;
			if (player_y >= SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
				player_y = SCREEN_HEIGHT - SPRITE_HEIGHT / 2;
		}

		else {
			if (player_x > -SPRITE_WIDTH / 2)
				player_x -= PLAYER_SPEED;

			if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_UP || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT) {
				state = IDLE;

				player_left_godmode.Reset();
				player_left.Reset();

			}

			if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
				&& player_y > SPRITE_HEIGHT)
				player_y -= PLAYER_SPEED;
			if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
				&& player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2) 
				player_y += PLAYER_SPEED;
			else
				App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_left.GetCurrentFrame());

				
		}

		break;

	case RIGHT:

		if (godmode)
			App->render->Blit(6, player, App->render->camera.x + player_x - 1, App->render->camera.y + player_y - 1, { 0,1 }, &player_right_godmode.GetCurrentFrame());

		else
			App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_right.GetCurrentFrame());

		App->render->Blit(5, player, App->render->camera.x + player_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_right);

		if (App->input->HasController(1)) {
			if (player_x < SCREEN_WIDTH - SPRITE_WIDTH / 2)
				player_x += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX);

			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX) <= 0.3) {
				state = IDLE;

				player_left_godmode.Reset();
				player_left.Reset();

			}

			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY) > 0.3)
				player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY);
			else {
				App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_left.GetCurrentFrame());
				if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY) < -0.3)
					player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY);
			}

			if (player_y <= SPRITE_HEIGHT)
				player_y = SPRITE_HEIGHT;
			if (player_y >= SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
				player_y = SCREEN_HEIGHT - SPRITE_HEIGHT / 2;
		}

		else {
			if (player_x < SCREEN_WIDTH - SPRITE_WIDTH / 2)
				player_x += PLAYER_SPEED;

			if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_UP || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT) {
				state = IDLE;

				player_right_godmode.Reset();
				player_right.Reset();

			}
			if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
				&& player_y > SPRITE_HEIGHT)
				player_y -= PLAYER_SPEED;
			if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
				&& player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2) 
				player_y += PLAYER_SPEED;
			else
				App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_left.GetCurrentFrame());

		}
		break;

	case FORWARD:

		if (godmode)
			App->render->Blit(6, player, App->render->camera.x + player_x - 1, App->render->camera.y + player_y - 1, { 0,1 }, &player_sprite_godmode);
		else
			App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite);
		
		App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_fire_forward.GetCurrentFrame());
		App->render->Blit(5, player, App->render->camera.x + player_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_idle);

		if (App->input->HasController(1)) {
			if (player_y > SPRITE_HEIGHT)
				player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY);

			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY) >= -0.3)
				state = IDLE;
			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX) > 0.3)
				state = RIGHT;
			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX) < -0.3)
				state = LEFT;
		}

		else {
			if (player_y > SPRITE_HEIGHT)
				player_y -= PLAYER_SPEED;

			if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_UP || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
				state = IDLE;
			if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT))
				state = LEFT;
			if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT))
				state = RIGHT;
		}

		break;

	case STOP:

		if (godmode)
			App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite_godmode);
		else
			App->render->Blit(6, player, App->render->camera.x + player_x, App->render->camera.y + player_y, { 0,1 }, &player_sprite);

		App->render->Blit(5, player, App->render->camera.x + player_x + SPRITE_WIDTH/2 + SHADOW_DISTANCE_X, App->render->camera.y + player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_idle);

		if (App->input->HasController(1)) {
			if (player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
				player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY);

			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTY) <= 0.3)
				state = IDLE;
			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX) > 0.3)
				state = RIGHT;
			if (App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_LEFTX) < -0.3)
				state = LEFT;
		}

		else {
			if (player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
				player_y += PLAYER_SPEED;

			if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_UP || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
				state = IDLE;
			if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT))
				state = LEFT;
			if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT))
				state = RIGHT;
		}
		break;
	}
	
	if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_REPEAT || App->input->GetControllerAxis(1, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 0) && sdl_clock > sdl_shot) {
		switch (current_bonus) {
		case MEDAL_BONUS:
			App->player->AddScore(300);
		case RED_BONUS:
			sdl_shot = sdl_clock + LASER_COOLDOWN;
			switch (amount_bonus) {
			case 0:
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 18, App->render->camera.y + player_y + 16);
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 35, App->render->camera.y + player_y + 16);
				break;
			case 1:
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 7, App->render->camera.y + player_y + 16);
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 16, App->render->camera.y + player_y + 16);

				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 34, App->render->camera.y + player_y + 16);
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 43, App->render->camera.y + player_y + 16);
				break;
			case 2:
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x, App->render->camera.y + player_y + 20, { -1,-4 });
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 15, App->render->camera.y + player_y + 16, { -1,-4 });
				
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 18, App->render->camera.y + player_y + 16);
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 35, App->render->camera.y + player_y + 16);

				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + SPRITE_WIDTH - 15, App->render->camera.y + player_y + 16, { 1,-4 });
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + SPRITE_WIDTH, App->render->camera.y + player_y + 20, { 1,-4 });
				break;

			case 3:
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x, App->render->camera.y + player_y + 20, { -1,-4 });
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 10, App->render->camera.y + player_y + 18, { -1,-4 });
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 20, App->render->camera.y + player_y + 16, { -1,-4 });

				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 16, App->render->camera.y + player_y);
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 27, App->render->camera.y + player_y);
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + 38, App->render->camera.y + player_y);

				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + SPRITE_WIDTH - 20, App->render->camera.y + player_y + 16, { 1,-4 });
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + SPRITE_WIDTH - 10, App->render->camera.y + player_y + 18, { 1,-4 });
				App->particles->AddParticle(AUTOSHOT, App->render->camera.x + player_x + SPRITE_WIDTH, App->render->camera.y + player_y + 20, { 1,-4 });
				break;
	

			}
			App->audio->PlaySFX(laser_sfx);
			break;
		case BLUE_BONUS:
			switch (amount_bonus) {
			case 0:
				sdl_shot = sdl_clock + LASER_COOLDOWN / 2;
				App->particles->AddParticle(LASERSHOT, App->render->camera.x + player_x + SPRITE_WIDTH / 2 - 1, App->render->camera.y + player_y);
				break;
			case 1:
				sdl_shot = sdl_clock + LASER_COOLDOWN / 2;
				App->particles->AddParticle(LASERSHOT, App->render->camera.x + player_x + SPRITE_WIDTH / 2 - 1, App->render->camera.y + player_y);
				App->particles->AddParticle(LASERSHOT, App->render->camera.x + player_x + SPRITE_WIDTH / 2 + 1, App->render->camera.y + player_y);
				break;

			case 2:
				sdl_shot = sdl_clock + LASER_COOLDOWN / 3;
				App->particles->AddParticle(LASERSHOT, App->render->camera.x + player_x + SPRITE_WIDTH / 2 - 1, App->render->camera.y + player_y);
				App->particles->AddParticle(LASERSHOT, App->render->camera.x + player_x + SPRITE_WIDTH / 2 + 1, App->render->camera.y + player_y);
				break;
			case 3:
				sdl_shot = sdl_clock + LASER_COOLDOWN / 3;
				App->particles->AddParticle(LASERSHOT, App->render->camera.x + player_x + SPRITE_WIDTH / 2 - 2, App->render->camera.y + player_y);
				App->particles->AddParticle(LASERSHOT, App->render->camera.x + player_x + SPRITE_WIDTH / 2, App->render->camera.y + player_y);
				App->particles->AddParticle(LASERSHOT, App->render->camera.x + player_x + SPRITE_WIDTH / 2 + 2, App->render->camera.y + player_y);
				break;

			}
			
			App->audio->PlaySFX(laser_sfx);
			break;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_F2] == KEY_DOWN || App->input->GetControllerButton(1, SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN)
	{
		godmode = !godmode;
	}

	App->collision->SetPosition(player_collider, App->render->camera.x + player_x, App->render->camera.y + player_y);

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
	player_left.CleanUp();
	player_right.CleanUp();
	player_left_godmode.CleanUp();
	player_right_godmode.CleanUp();
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
		if (c2->type == COLLIDER_ENEMY_AIR || c2->type == COLLIDER_ENEMY_SHOT)	
		{
			App->input->ShakeController(1, 2000, 1.0);
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
	if (!godmode)
		score += score_add;
}

void ModulePlayer::AddBonus(BONUS_TYPE type, Collider* col) {
	AddScore(500);
	if (type != current_bonus) {
		current_bonus = type;
		amount_bonus--;
		if (amount_bonus < 0)
			amount_bonus = 0;
	}
	else {
		if (amount_bonus < 3)
			++amount_bonus;
	}
}
