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

	player = App->textures->Load("revamp_spritesheets/player_spritesheet.png");
	laser_sfx = App->audio->LoadSFX("sfx/shot_regular.wav");

	sdl_shot = 0;

	state = IDLE;

	
	player_collider = App->collision->AddCollider({0, 0, 60, 50}, COLLIDER_PLAYER, this);

	return ret;
}

update_status ModulePlayer::Update() {
	sdl_clock = SDL_GetTicks();
	switch (state) {
	case IDLE:
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite);
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
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_left);

		if (player_x > -SPRITE_WIDTH / 2)
			player_x -= PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_UP || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT)
			state = IDLE;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
			&& player_y > 0)
			player_y -= PLAYER_SPEED;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
			&& player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
			player_y += PLAYER_SPEED;
		break;

	case RIGHT:
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_right);

		if (player_x < SCREEN_WIDTH - SPRITE_WIDTH / 2)
			player_x += PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_UP || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT)
			state = IDLE;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
			&& player_y> 0)
			player_y -= PLAYER_SPEED;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
			&& player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
			player_y += PLAYER_SPEED;
		break;

	case FORWARD:
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite);
		if (player_y > 0 - SPRITE_HEIGHT / 2)
			player_y -= PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_UP || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
			state = IDLE;
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT))
			state = LEFT;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT))
			state = RIGHT;
		break;

	case STOP:
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite);

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

	player_collider->SetPos(App->render->camera.x + player_x, App->render->camera.y + player_y);

	
	
		
	

	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;

	if (player_collider != nullptr)
		player_collider->to_delete = true;

	return ret;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	
	if (c1->type == COLLIDER_ENEMY || c2->type == COLLIDER_ENEMY) {
		App->particles->AddParticle(EXPLOSION, App->render->camera.x + player_x, App->render->camera.y + player_y);
		App->transition->Transition(App->stage1, App->intro, 0.8f);
	}
}
