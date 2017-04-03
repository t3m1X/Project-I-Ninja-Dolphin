#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"

ModulePlayer::ModulePlayer() 
{}

ModulePlayer::~ModulePlayer() 
{}

bool ModulePlayer::Start() {
	bool ret = true;

	player_x = SCREEN_WIDTH / 2 - SPRITE_SIZE;
	player_y = SCREEN_HEIGHT / 2 - SPRITE_SIZE;

	player_sprite.h = player_sprite.w = SPRITE_SIZE;
	player_sprite.x = 0;
	player_sprite.y = 34;

	player_sprite_left.h = player_sprite_left.w = SPRITE_SIZE;
	player_sprite_left.x = 101;
	player_sprite_left.y = 34;

	player_sprite_right.h = player_sprite_right.w = SPRITE_SIZE;
	player_sprite_right.x = 70;
	player_sprite_right.y = 34;

	player = App->textures->Load("spritesheets/player/spritesheet_player.png");

	state = idle;

	return ret;
}

update_status ModulePlayer::Update() {
	switch (state) {
	case idle:
		App->render->Blit(player, player_x, player_y, 2, &player_sprite);
		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT))
			state = left;
		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT))
			state = right;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT))
			state = forward;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT))
			state = stop;
		break;

	case left:
		App->render->Blit(player, player_x, player_y, 2, &player_sprite_left);

		if (player_x > 0)
			player_x -= PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_UP || App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT)
			state = idle;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT))
			player_y -= PLAYER_SPEED;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT))
			player_y += PLAYER_SPEED;
		break;

	case right:
		App->render->Blit(player, player_x, player_y, 2, &player_sprite_right);

		if (player_x < SCREEN_WIDTH - SPRITE_SIZE)
			player_x += PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_UP || App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT)
			state = idle;
		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT))
			player_y -= PLAYER_SPEED;
		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT && !(App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT))
			player_y += PLAYER_SPEED;
		break;

	case forward:
		App->render->Blit(player, player_x, player_y, 2, &player_sprite);
		if (player_y > 0)
			player_y -= PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_UP] == KEY_UP || App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT)
			state = idle;
		break;

	case stop:
		App->render->Blit(player, player_x, player_y, 2, &player_sprite);

		if (player_y < SCREEN_HEIGHT - SPRITE_SIZE)
			player_y += PLAYER_SPEED;

		if (App->input->keyboard[SDL_SCANCODE_DOWN] == KEY_UP || App->input->keyboard[SDL_SCANCODE_UP] == KEY_REPEAT)
			state = idle;
		break;
	}
	
	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN) {

		//Bullet must travel, for now its static
		//-----------------------

			App->particles->AddParticle(App->particles->autoattack, player_x + 28, player_y - 30);
			App->particles->AddParticle(App->particles->autoattack, player_x + 34, player_y - 30);

			App->particles->AddParticle(App->particles->autoattack, player_x + 28, player_y - 60);
			App->particles->AddParticle(App->particles->autoattack, player_x + 34, player_y - 60);
		
			

		
		
	}

	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;
	return ret;
}
