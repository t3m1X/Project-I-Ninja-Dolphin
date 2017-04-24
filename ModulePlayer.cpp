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

	player_fire_forward.SetUp(0, 49, 57, 66, 5, 5, "0,1,2,3,4");
	player_fire_forward.speed = 0.2f;
	player_fire_left.SetUp(0, 115, 57, 66, 5, 5, "0,1,2,3,4");
	player_fire_left.speed = 0.2f;
	player_fire_right.SetUp(0, 181, 57, 66, 5, 5, "0,1,2,3,4");
	player_fire_right.speed = 0.2f;

	return ret;
}

update_status ModulePlayer::Update() {
	sdl_clock = SDL_GetTicks();
	switch (state) {
	case IDLE:
		if (godmode == true)
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_godmode);
		}
		else
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite);
		}
		
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_fire_forward.GetCurrentFrame());

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
		if (godmode == true)
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_godmode_left);
		}
		else
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_left);
		}
		
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_fire_left.GetCurrentFrame());


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

		if (godmode == true)
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_godmode_right);
		}
		else
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_right);
		}
		
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_fire_right.GetCurrentFrame());


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

		if (godmode == true)
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_godmode);
		}
		else
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite);
		}
		
		App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_fire_forward.GetCurrentFrame());
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

		if (godmode == true)
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite_godmode);
		}
		else
		{
			App->render->Blit(player, App->render->camera.x + player_x, App->render->camera.y + player_y, &player_sprite);
		}
		

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
		if (godmode == false)
		{
			godmode = true;
		}
		else
		{
			godmode = false;
		}
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

	if (godmode == true)
	{
		return;
	}

	else
	{
		if (c1->type == COLLIDER_ENEMY || c2->type == COLLIDER_ENEMY)
		{
			App->particles->AddParticle(EXPLOSION, App->render->camera.x + player_x, App->render->camera.y + player_y);
			App->player->Disable();
			App->transition->Transition(App->stage1, App->intro, 0.8f);
		}

	}
}
