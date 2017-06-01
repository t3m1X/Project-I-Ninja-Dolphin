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
#include "ModuleLoseScreen.h"

void NumberToChar(int number, char* string)
{
	string += 8;
	*string-- = '\0';

	for (int i = 0; i < 8; ++i, number /= 10, --string) {
		if (i == 0 && number == 0)
			*string = '0';
		else if (number != 0)
			*string = '0' + number % 10;
		else
			*string = ' ';
	}
}

ModulePlayer::ModulePlayer() 
{
	players[1].state = OFF;
}

ModulePlayer::~ModulePlayer() 
{}

bool ModulePlayer::Start() {
	bool ret = true;

	players[0].player_world_x = App->render->camera.x + SCREEN_WIDTH / 2 - SPRITE_WIDTH / 2;
	players[0].player_y = SCREEN_HEIGHT / 2 + SPRITE_HEIGHT;

	players[1].player_world_x = App->render->camera.x + SCREEN_WIDTH / 2 - SPRITE_WIDTH / 2 + SPRITE_WIDTH + 20;
	players[1].player_y = SCREEN_HEIGHT / 2 + SPRITE_HEIGHT;

	players[0].state = IDLE;
	players[0].player_collider = App->collision->AddCollider({ 0, 0, 20, 35 }, COLLIDER_PLAYER, this);
	if (players[1].state != OFF) {
		players[0].player_world_x -= SPRITE_WIDTH + 20;
		players[1].state = IDLE;
		players[1].player_collider = App->collision->AddCollider({ 0, 0, 20, 35 }, COLLIDER_PLAYER, this);
	}

	//Setting keyboard inputs
	//Player 1
	players[0].inputs[PI_BACK] = SDL_SCANCODE_DOWN;
	players[0].inputs[PI_FORWARD] = SDL_SCANCODE_UP;
	players[0].inputs[PI_LEFT] = SDL_SCANCODE_LEFT;
	players[0].inputs[PI_RIGHT] = SDL_SCANCODE_RIGHT;
	players[0].inputs[PI_SHOOT] = SDL_SCANCODE_RCTRL;
	players[0].inputs[PI_BOMB] = SDL_SCANCODE_RSHIFT;
	players[0].inputs[PI_GODMODE] = SDL_SCANCODE_F2;
	//Player 2
	players[1].inputs[PI_BACK] = SDL_SCANCODE_S;
	players[1].inputs[PI_FORWARD] = SDL_SCANCODE_W;
	players[1].inputs[PI_LEFT] = SDL_SCANCODE_A;
	players[1].inputs[PI_RIGHT] = SDL_SCANCODE_D;
	players[1].inputs[PI_SHOOT] = SDL_SCANCODE_SPACE;
	players[1].inputs[PI_BOMB] = SDL_SCANCODE_B;
	players[1].inputs[PI_GODMODE] = SDL_SCANCODE_F8;

	for (int i = 0; i < 2; ++i) {
		players[i].animations[AN_IDLE].SetUp(0 + 391 * i, 0, SPRITE_WIDTH, SPRITE_HEIGHT, 1, 1, "0");
		players[i].animations[AN_IDLE_GOD].SetUp(0 + 391 * i, 416, SPRITE_WIDTH + 2, SPRITE_HEIGHT + 1, 1, 1, "0");

		players[i].animations[AN_LEFT].SetUp(57 + 391 * i, 0, SPRITE_WIDTH, SPRITE_HEIGHT, 2, 2, "0,1");
		players[i].animations[AN_LEFT].speed = 0.05f;
		players[i].animations[AN_LEFT].loop = false;

		players[i].animations[AN_LEFT_GOD].SetUp(61 + 391 * i, 416, SPRITE_WIDTH, SPRITE_HEIGHT + 1, 2, 2, "0,1");
		players[i].animations[AN_LEFT_GOD].speed = 0.05f;
		players[i].animations[AN_LEFT_GOD].loop = false;

		players[i].animations[AN_RIGHT].SetUp(171 + 391 * i, 0, SPRITE_WIDTH, SPRITE_HEIGHT, 2, 2, "0,1");
		players[i].animations[AN_RIGHT].speed = 0.05f;
		players[i].animations[AN_RIGHT].loop = false;

		players[i].animations[AN_RIGHT_GOD].SetUp(177 + 391 * i, 416, SPRITE_WIDTH, SPRITE_HEIGHT + 1, 2, 2, "0,1");
		players[i].animations[AN_RIGHT_GOD].speed = 0.05f;
		players[i].animations[AN_RIGHT_GOD].loop = false;

		players[i].animations[AN_FIRE].SetUp(0 + 391 * i, 49, SPRITE_WIDTH, SPRITE_HEIGHT + 19, 5, 5, "0,1,2,3,4");
		players[i].animations[AN_FIRE].speed = 0.2f;

		players[i].animations[AN_FIRE_LEFT].SetUp(0 + 391 * i, 117, SPRITE_WIDTH, SPRITE_HEIGHT + 19, 5, 10, "0,1,2,3,5,6,7,8,9");
		players[i].animations[AN_FIRE_LEFT].LoopStart(4);
		players[i].animations[AN_FIRE_LEFT].speed = 0.2f;

		players[i].animations[AN_FIRE_RIGHT].SetUp(0 + 391 * i, 185, SPRITE_WIDTH, SPRITE_HEIGHT + 19, 5, 10, "0,1,2,3,5,6,7,8,9");
		players[i].animations[AN_FIRE_RIGHT].LoopStart(4);
		players[i].animations[AN_FIRE_RIGHT].speed = 0.2f;


		players[i].animations[AN_LIVE].SetUp(285 + 391 * i, 29, 16, 16, 1, 1, "0");

		players[i].animations[AN_SHOOTING_BLUE].SetUp(391, 468, SPRITE_WIDTH, SPRITE_HEIGHT, 4, 4, "0,1,2,3");
		players[i].animations[AN_SHOOTING_BLUE].speed = 0.4f;
		players[i].animations[AN_SHOOTING_BLUE].loop = false;
		players[i].animations[AN_SHOOTING_BLUE].SetFrame(3);

		players[i].animations[AN_SHOOTING_RED].SetUp(0, 468, SPRITE_WIDTH, SPRITE_HEIGHT, 4, 4, "0,1,2,3");
		players[i].animations[AN_SHOOTING_RED].speed = 0.4f;
		players[i].animations[AN_SHOOTING_RED].loop = false;
		players[i].animations[AN_SHOOTING_RED].SetFrame(3);

		if (players[i].lives <= 0) {
			players[i].lives = 3;
			players[i].score = 0;
		}

		players[i].sdl_shot = 0;
		if (players[i].current_bonus == NULL)
			players[i].current_bonus = RED_BONUS;
	}

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
	font = App->fonts->LoadFont("fonts/PrStart.ttf", 8);

	return ret;
}

update_status ModulePlayer::Update() {

	sdl_clock = SDL_GetTicks();

	for (int i = 0; i < 2; ++i) {
		switch (players[i].state) {
		case OFF:
			if (App->input->HasController(i + 1)) {
				if (App->input->GetControllerButton(i + 1, SDL_CONTROLLER_BUTTON_START) == KEY_DOWN) {
					players[i].state = IDLE;
					players[i].player_collider = App->collision->AddCollider({ 0, 0, 20, 35 }, COLLIDER_PLAYER, this);
					players[i].player_world_x = players[0].player_world_x + 40;
					players[i].player_y = players[0].player_y;
				}
				else
					continue;
			}
			else {
				if (App->input->keyboard[players[i].inputs[PI_SHOOT]] == KEY_DOWN) {
					players[i].state = IDLE;
					players[i].player_collider = App->collision->AddCollider({ 0, 0, 20, 35 }, COLLIDER_PLAYER, this);
					players[i].player_world_x = players[0].player_world_x + 40;
					players[i].player_y = players[0].player_y;
				}
				else
					continue;
			}
			break;
		case DEAD:
			if (players[i].lives <= 0 || sdl_clock < players[i].sdl_respawn) {
				if (i == 0) {
					players[0].player_world_x = App->render->camera.x + SCREEN_WIDTH / 2 - SPRITE_WIDTH / 2;
					if (players[1].state != OFF)
						players[0].player_world_x -= SPRITE_WIDTH + 20;
					players[0].player_y = SCREEN_HEIGHT + 10;
				}
				else {
					players[1].player_world_x = App->render->camera.x + SCREEN_WIDTH / 2 - SPRITE_WIDTH / 2 + SPRITE_WIDTH + 20;
					players[1].player_y = SCREEN_HEIGHT + 10;
				}
			}
			else {
				if (players[i].player_y > SCREEN_HEIGHT / 2 + SPRITE_HEIGHT * 2)
					players[i].player_y -= PLAYER_SPEED;
				else {
					players[i].state = IDLE;
					players[i].player_collider = App->collision->AddCollider({ 0, 0, 20, 35 }, COLLIDER_PLAYER, this);
				}
				App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0, 1 }, &players[i].animations[AN_IDLE_GOD].GetCurrentFrame());
				App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_FIRE].GetCurrentFrame());
			}
			continue;
		case IDLE:
			if (players[i].godmode)
				App->render->Blit(6, player, players[i].player_world_x - 1, App->render->camera.y + players[i].player_y - 1, { 0, 1 }, &players[i].animations[AN_IDLE_GOD].GetCurrentFrame());

			else
				App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0, 1 }, &players[i].animations[AN_IDLE].GetCurrentFrame());

			App->render->Blit(5, player, players[i].player_world_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + players[i].player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_idle);
			App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_FIRE].GetCurrentFrame());

			if (App->input->HasController(i + 1)) {
				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX) < -0.3)
					players[i].state = LEFT;
				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX) > 0.3)
					players[i].state = RIGHT;
				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY) < -0.3)
					players[i].state = FORWARD;
				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY) > 0.3)
					players[i].state = STOP;
			}
			else {
				if (App->input->keyboard[players[i].inputs[PI_LEFT]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_RIGHT]] == KEY_REPEAT))
					players[i].state = LEFT;
				if (App->input->keyboard[players[i].inputs[PI_RIGHT]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_LEFT]] == KEY_REPEAT))
					players[i].state = RIGHT;
				if (App->input->keyboard[players[i].inputs[PI_FORWARD]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_BACK]] == KEY_REPEAT))
					players[i].state = FORWARD;
				if (App->input->keyboard[players[i].inputs[PI_BACK]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_FORWARD]] == KEY_REPEAT))
					players[i].state = STOP;
			}


			break;

		case LEFT:
			if (players[i].godmode)
				App->render->Blit(6, player, players[i].player_world_x - 1, App->render->camera.y + players[i].player_y - 1, { 0,1 }, &players[i].animations[AN_LEFT_GOD].GetCurrentFrame());

			else
				App->render->Blit(6, player, players[i].player_world_x - 1, App->render->camera.y + players[i].player_y - 1, { 0,1 }, &players[i].animations[AN_LEFT].GetCurrentFrame());

			App->render->Blit(5, player, players[i].player_world_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + players[i].player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_left);

			if (App->input->HasController(i + 1)) {
				if (players[i].player_world_x - App->render->camera.x > -SPRITE_WIDTH / 2)
					players[i].player_world_x += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX);

				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX) >= -0.3) {
					players[i].state = IDLE;

					players[i].animations[AN_LEFT_GOD].Reset();
					players[i].animations[AN_LEFT].Reset();

				}

				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY) > 0.3)
					players[i].player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY);
				else {
					App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_FIRE_LEFT].GetCurrentFrame());
					if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY) < -0.3)
						players[i].player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY);
				}

				if (App->render->camera.x > SCREEN_WIDTH / 2 - STAGE_WIDTH / 2 && (players[abs(i - 1)].state == OFF || players[abs(i - 1)].state == DEAD || players[abs(i - 1)].player_world_x - App->render->camera.x < SCREEN_WIDTH / 2)) {
					App->render->camera.x += (SCROLL_SPEED + 1) * App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX);
					if (App->render->camera.x < SCREEN_WIDTH / 2 - STAGE_WIDTH / 2)
							App->render->camera.x = SCREEN_WIDTH / 2 - STAGE_WIDTH / 2;
				}
				if (players[i].player_y <= SPRITE_HEIGHT)
					players[i].player_y = SPRITE_HEIGHT;
				if (players[i].player_y >= SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
					players[i].player_y = SCREEN_HEIGHT - SPRITE_HEIGHT / 2;
			}

			else {
				if (players[i].player_world_x - App->render->camera.x > -SPRITE_WIDTH / 2)
					players[i].player_world_x -= PLAYER_SPEED;

				if (App->input->keyboard[players[i].inputs[PI_LEFT]] == KEY_UP || App->input->keyboard[players[i].inputs[PI_RIGHT]] == KEY_REPEAT) {
					players[i].state = IDLE;

					players[i].animations[AN_LEFT_GOD].Reset();
					players[i].animations[AN_LEFT].Reset();

				}

				if (App->render->camera.x > SCREEN_WIDTH / 2 - STAGE_WIDTH / 2 && (players[abs(i - 1)].state == OFF || players[abs(i - 1)].state == DEAD || players[abs(i - 1)].player_world_x - App->render->camera.x < SCREEN_WIDTH / 2))
					App->render->camera.x -= SCROLL_SPEED;

				if (App->input->keyboard[players[i].inputs[PI_FORWARD]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_BACK]] == KEY_REPEAT)
					&& players[i].player_y > SPRITE_HEIGHT)
					players[i].player_y -= PLAYER_SPEED;
				if (App->input->keyboard[players[i].inputs[PI_BACK]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_FORWARD]] == KEY_REPEAT)
					&& players[i].player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
					players[i].player_y += PLAYER_SPEED;
				else
					App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_FIRE_LEFT].GetCurrentFrame());


			}

			break;

		case RIGHT:

			if (players[i].godmode)
				App->render->Blit(6, player, players[i].player_world_x - 1, App->render->camera.y + players[i].player_y - 1, { 0,1 }, &players[i].animations[AN_RIGHT_GOD].GetCurrentFrame());

			else
				App->render->Blit(6, player, players[i].player_world_x - 1, App->render->camera.y + players[i].player_y - 1, { 0,1 }, &players[i].animations[AN_RIGHT].GetCurrentFrame());

			App->render->Blit(5, player, players[i].player_world_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + players[i].player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_right);

			if (App->input->HasController(i + 1)) {
				if (players[i].player_world_x - App->render->camera.x < SCREEN_WIDTH - SPRITE_WIDTH / 2)
					players[i].player_world_x += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX);

				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX) <= 0.3) {
					players[i].state = IDLE;

					players[i].animations[AN_RIGHT_GOD].Reset();
					players[i].animations[AN_RIGHT].Reset();

				}
				if (App->render->camera.x < STAGE_WIDTH / 2 - SCREEN_WIDTH / 2 && (players[abs(i - 1)].state == OFF || players[abs(i - 1)].state == DEAD || players[abs(i - 1)].player_world_x - App->render->camera.x > SCREEN_WIDTH / 2)) {
					App->render->camera.x += (SCROLL_SPEED + 1) * App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX);
					if (App->render->camera.x < SCREEN_WIDTH / 2 - STAGE_WIDTH / 2)
						App->render->camera.x = SCREEN_WIDTH / 2 - STAGE_WIDTH / 2;
				}

				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY) > 0.3)
					players[i].player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY);
				else {
					App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_FIRE_RIGHT].GetCurrentFrame());
					if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY) < -0.3)
						players[i].player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY);
				}

				if (players[i].player_y <= SPRITE_HEIGHT)
					players[i].player_y = SPRITE_HEIGHT;
				if (players[i].player_y >= SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
					players[i].player_y = SCREEN_HEIGHT - SPRITE_HEIGHT / 2;
			}

			else {
				if (players[i].player_world_x - App->render->camera.x < SCREEN_WIDTH - SPRITE_WIDTH / 2)
					players[i].player_world_x += PLAYER_SPEED;

				if (App->input->keyboard[players[i].inputs[PI_RIGHT]] == KEY_UP || App->input->keyboard[players[i].inputs[PI_LEFT]] == KEY_REPEAT) {
					players[i].state = IDLE;

					players[i].animations[AN_RIGHT_GOD].Reset();
					players[i].animations[AN_RIGHT].Reset();

				}

				if (App->render->camera.x < STAGE_WIDTH / 2 - SCREEN_WIDTH / 2 && (players[abs(i - 1)].state == OFF || players[abs(i - 1)].state == DEAD || players[abs(i - 1)].player_world_x - App->render->camera.x > SCREEN_WIDTH / 2)) 
					App->render->camera.x += SCROLL_SPEED;

				if (App->input->keyboard[players[i].inputs[PI_FORWARD]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_BACK]] == KEY_REPEAT)
					&& players[i].player_y > SPRITE_HEIGHT)
					players[i].player_y -= PLAYER_SPEED;
				if (App->input->keyboard[players[i].inputs[PI_BACK]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_FORWARD]] == KEY_REPEAT)
					&& players[i].player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
					players[i].player_y += PLAYER_SPEED;
				else
					App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_FIRE_RIGHT].GetCurrentFrame());
			}
			break;

		case FORWARD:

			if (players[i].godmode)
				App->render->Blit(6, player, players[i].player_world_x - 1, App->render->camera.y + players[i].player_y - 1, { 0, 1 }, &players[i].animations[AN_IDLE_GOD].GetCurrentFrame());

			else
				App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0, 1 }, &players[i].animations[AN_IDLE].GetCurrentFrame());

			App->render->Blit(5, player, players[i].player_world_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + players[i].player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_idle);
			App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_FIRE].GetCurrentFrame());

			if (App->input->HasController(i + 1)) {
				if (players[i].player_y > SPRITE_HEIGHT)
					players[i].player_y += (PLAYER_SPEED + 1) *App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY);

				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY) >= -0.3)
					players[i].state = IDLE;
				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX) > 0.3)
					players[i].state = RIGHT;
				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX) < -0.3)
					players[i].state = LEFT;
			}

			else {
				if (players[i].player_y > SPRITE_HEIGHT)
					players[i].player_y -= PLAYER_SPEED;

				if (App->input->keyboard[players[i].inputs[PI_FORWARD]] == KEY_UP || App->input->keyboard[players[i].inputs[PI_BACK]] == KEY_REPEAT)
					players[i].state = IDLE;
				if (App->input->keyboard[players[i].inputs[PI_LEFT]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_RIGHT]] == KEY_REPEAT))
					players[i].state = LEFT;
				if (App->input->keyboard[players[i].inputs[PI_RIGHT]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_LEFT]] == KEY_REPEAT))
					players[i].state = RIGHT;
			}

			break;

		case STOP:

			if (players[i].godmode)
				App->render->Blit(6, player, players[i].player_world_x - 1, App->render->camera.y + players[i].player_y - 1, { 0, 1 }, &players[i].animations[AN_IDLE_GOD].GetCurrentFrame());
			else
				App->render->Blit(6, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0, 1 }, &players[i].animations[AN_IDLE].GetCurrentFrame());

			App->render->Blit(5, player, players[i].player_world_x + SPRITE_WIDTH / 2 + SHADOW_DISTANCE_X, App->render->camera.y + players[i].player_y + SPRITE_HEIGHT / 2 + SHADOW_DISTANCE_Y, { 0,1 }, &shadow_idle);

			if (App->input->HasController(i + 1)) {
				if (players[i].player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
					players[i].player_y += (PLAYER_SPEED + 1) * App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY);

				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTY) <= 0.3)
					players[i].state = IDLE;
				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX) > 0.3)
					players[i].state = RIGHT;
				if (App->input->GetControllerAxis(i + 1, SDL_CONTROLLER_AXIS_LEFTX) < -0.3)
					players[i].state = LEFT;
			}

			else {
				if (players[i].player_y < SCREEN_HEIGHT - SPRITE_HEIGHT / 2)
					players[i].player_y += PLAYER_SPEED;

				if (App->input->keyboard[players[i].inputs[PI_BACK]] == KEY_UP || App->input->keyboard[players[i].inputs[PI_FORWARD]] == KEY_REPEAT)
					players[i].state = IDLE;
				if (App->input->keyboard[players[i].inputs[PI_LEFT]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_RIGHT]] == KEY_REPEAT))
					players[i].state = LEFT;
				if (App->input->keyboard[players[i].inputs[PI_RIGHT]] == KEY_REPEAT && !(App->input->keyboard[players[i].inputs[PI_LEFT]] == KEY_REPEAT))
					players[i].state = RIGHT;
			}
			break;
		}


		if (((App->input->keyboard[players[i].inputs[PI_SHOOT]] == KEY_DOWN || App->input->GetControllerButton(i + 1, SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)) ||
			((App->input->keyboard[players[i].inputs[PI_SHOOT]] == KEY_REPEAT || App->input->GetControllerButton(i + 1, SDL_CONTROLLER_BUTTON_A) == KEY_REPEAT) && sdl_clock > players[i].sdl_shot)) {
			players[i].sdl_shot = sdl_clock + SHOT_COOLDOWN;
			switch (players[i].current_bonus) {
			case RED_BONUS:
				players[i].animations[AN_SHOOTING_RED].Reset();
				players[i].animations[AN_SHOOTING_BLUE].SetFrame(3);
				switch (players[i].amount_bonus) {
				case 0:
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 18, App->render->camera.y + players[i].player_y + 16, { 999,999 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 35, App->render->camera.y + players[i].player_y + 16, { 999,999 }, i == 0);
					break;
				case 1:
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 7, App->render->camera.y + players[i].player_y + 16, { 999,999 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 16, App->render->camera.y + players[i].player_y + 16, { 999,999 }, i == 0);

					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 34, App->render->camera.y + players[i].player_y + 16, { 999,999 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 43, App->render->camera.y + players[i].player_y + 16, { 999,999 }, i == 0);
					break;
				case 2:
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x, App->render->camera.y + players[i].player_y + 20, { -1,-4 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 15, App->render->camera.y + players[i].player_y + 16, { -1,-4 }, i == 0);

					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 18, App->render->camera.y + players[i].player_y + 16, { 999,999 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 35, App->render->camera.y + players[i].player_y + 16, { 999,999 }, i == 0);

					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + SPRITE_WIDTH - 15, App->render->camera.y + players[i].player_y + 16, { 1,-4 }, i== 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + SPRITE_WIDTH, App->render->camera.y + players[i].player_y + 20, { 1,-4 }, i == 0);
					break;

				case 3:
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x, App->render->camera.y + players[i].player_y + 20, { -1,-4 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 10, App->render->camera.y + players[i].player_y + 18, { -1,-4 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 20, App->render->camera.y + players[i].player_y + 16, { -1,-4 }, i == 0);

					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 16, App->render->camera.y + players[i].player_y, { 999,999 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 27, App->render->camera.y + players[i].player_y, { 999,999 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + 38, App->render->camera.y + players[i].player_y, { 999,999 }, i == 0);

					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + SPRITE_WIDTH - 20, App->render->camera.y + players[i].player_y + 16, { 1,-4 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + SPRITE_WIDTH - 10, App->render->camera.y + players[i].player_y + 18, { 1,-4 }, i == 0);
					App->particles->AddParticle(AUTOSHOT, players[i].player_world_x + SPRITE_WIDTH, App->render->camera.y + players[i].player_y + 20, { 1,-4 }, i == 0);
					break;


				}
				break;
			case BLUE_BONUS:
				players[i].animations[AN_SHOOTING_BLUE].Reset();
				players[i].animations[AN_SHOOTING_RED].SetFrame(3);
				switch (players[i].amount_bonus) {

				case 0:
					for (int j = 0; j < 3; ++j)
						App->particles->AddParticle(LASERSHOT, players[i].player_world_x + SPRITE_WIDTH / 2 - 1, App->render->camera.y + players[i].player_y - 30, { 0,-1 }, i == 0, LASER_COOLDOWN * j);
					break;

				case 1:
					for (int j = 0; j < 9; ++j)
						App->particles->AddParticle(LASERSHOT, players[i].player_world_x + SPRITE_WIDTH / 2 - 1, App->render->camera.y + players[i].player_y - 30, { 0,-1 }, i == 0, LASER_COOLDOWN * j);
					break;

				case 2:
					for (int j = 0; j < 9; ++j)
						App->particles->AddParticle(LASERBIGSHOT, players[i].player_world_x + 12, App->render->camera.y + players[i].player_y, { 0,-1 }, i == 0, LASER_COOLDOWN * j);

					for (int j = 0; j < 9; ++j)
						App->particles->AddParticle(LASERBIGSHOT, players[i].player_world_x + 33, App->render->camera.y + players[i].player_y, { 0,-1 }, i == 0, LASER_COOLDOWN * j);

					break;
				case 3:
					App->particles->AddParticle(BIGASSLASER, players[i].player_world_x + SPRITE_WIDTH / 2 - 37 / 2, App->render->camera.y + players[i].player_y, { 0,-1 }, i == 0);
					players[i].sdl_shot = sdl_clock + BIG_LASER_COOLDOWN;
					break;
				}


				break;
			}
		}

		if (!players[i].animations[AN_SHOOTING_RED].Finished())
			App->render->Blit(5, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_SHOOTING_RED].GetCurrentFrame());
		if (!players[i].animations[AN_SHOOTING_BLUE].Finished())
			App->render->Blit(5, player, players[i].player_world_x, App->render->camera.y + players[i].player_y, { 0,1 }, &players[i].animations[AN_SHOOTING_BLUE].GetCurrentFrame());
		
		if (players[i].score > highscore)
			highscore = players[i].score;

		if (App->input->keyboard[players[i].inputs[PI_GODMODE]] == KEY_DOWN || App->input->GetControllerButton(i + 1, SDL_CONTROLLER_BUTTON_BACK) == KEY_DOWN) {
			players[i].godmode = !players[i].godmode;
		}

		if (players[i].player_collider != nullptr)
			App->collision->SetPosition(players[i].player_collider, players[i].player_world_x + 18, App->render->camera.y + players[i].player_y + 10);
	}

	//Score printing
	char number[9];
	App->fonts->WriteText(font, "PLAYER 1", App->render->camera.x + 35, App->render->camera.y + 8, { 0,0,0 });
	App->fonts->WriteText(font, "PLAYER 1", App->render->camera.x + 35, App->render->camera.y + 5, { 255,255,255 });

	NumberToChar(players[0].score, number);
	App->fonts->WriteText(font, number, App->render->camera.x + 35, App->render->camera.y + 20, { 0,0,0 });
	App->fonts->WriteText(font, number, App->render->camera.x + 35, App->render->camera.y + 17, { 255,255,255 });

	App->fonts->WriteText(font, "HIGHSCORE", App->render->camera.x + 185, App->render->camera.y + 8, { 0,0,0 });
	App->fonts->WriteText(font, "HIGHSCORE", App->render->camera.x + 185, App->render->camera.y + 5, { 255,255,255 });

	NumberToChar(highscore, number);
	App->fonts->WriteText(font, number, App->render->camera.x + 191, App->render->camera.y + 20, { 0,0,0 });
	App->fonts->WriteText(font, number, App->render->camera.x + 191, App->render->camera.y + 17, { 255,255,255 });

	App->fonts->WriteText(font, "PLAYER 2", App->render->camera.x + 335, App->render->camera.y + 8, { 0,0,0 });
	App->fonts->WriteText(font, "PLAYER 2", App->render->camera.x + 335, App->render->camera.y + 5, { 255,255,255 });

	NumberToChar(players[1].score, number);
	App->fonts->WriteText(font, number, App->render->camera.x + 335, App->render->camera.y + 20, { 0,0,0 });
	App->fonts->WriteText(font, number, App->render->camera.x + 335, App->render->camera.y + 17, { 255,255,255 });

	//Printing lives
	//--Player 1
	for (int i = 1; i < players[0].lives; ++i)
		App->render->Blit(7, player, App->render->camera.x + 35 + 17 * (i - 1), App->render->camera.y + 32, { 0,1 }, &players[0].animations[AN_LIVE].GetCurrentFrame());
	//--Player 2
	if (players[1].state != OFF) {
		for (int i = 1; i < players[1].lives; ++i)
			App->render->Blit(7, player, App->render->camera.x + SCREEN_WIDTH - 65 - 17 * (i - 1), App->render->camera.y + 32, { 0,1 }, &players[1].animations[AN_LIVE].GetCurrentFrame());
	}
	else {
		if (App->input->HasController(2)) {
			App->fonts->WriteText(font, "PRESS START", App->render->camera.x + 325, App->render->camera.y + 42, { 0,0,0 });
			App->fonts->WriteText(font, "PRESS START", App->render->camera.x + 325, App->render->camera.y + 39, { 255,255,255 });
		}
		else {
			App->fonts->WriteText(font, "PRESS SPACE", App->render->camera.x + 325, App->render->camera.y + 42, { 0,0,0 });
			App->fonts->WriteText(font, "PRESS SPACE", App->render->camera.x + 325, App->render->camera.y + 39, { 255,255,255 });
		}
	}


	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp() {
	bool ret = true;

	for (int i = 0; i < 2; i++) {
		if (players[i].player_collider != nullptr)
			App->collision->EraseCollider(players[i].player_collider);

		for (int j = 0; j < AN_MAX; ++j)
			players[i].animations[j].CleanUp();
	}

	App->textures->Unload(player);

	if (font != nullptr) {
		App->fonts->EraseFont(font);
		font = nullptr;
	}

	return ret;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	for (int i = 0; i < 2; ++i) {
		if (c1 == players[i].player_collider) {
			if (players[i].godmode == true) {
				return;
			}

			else {
				if (c2->type == COLLIDER_ENEMY_AIR || c2->type == COLLIDER_ENEMY_SHOT) {
					App->input->ShakeController(i + 1, 2000, 1.0);
					App->particles->AddParticle(PLAYER_EXPLOSION, players[i].player_world_x - 29, App->render->camera.y + players[i].player_y - 26, { 999,999 }, i == 0);
					SpawnBits(i == 0);
					players[i].current_bonus = RED_BONUS;
					players[i].amount_bonus = 0;
					players[i].lives--;
					players[i].state = DEAD;
					players[i].sdl_respawn = sdl_clock + 2000;
					App->collision->EraseCollider(players[i].player_collider);
					players[i].player_collider = nullptr;
				}

			}
		}
	}

	if (players[0].lives <= 0 && (players[1].lives <= 0 || players[1].state == OFF)) {
		Disable();
		App->transition->Transition(App->stage1, App->losescreen, 0.8f);
	}
}

iPoint ModulePlayer::GetPos()
{
	return { players[0].player_world_x, App->render->camera.y + players[0].player_y };
}

void ModulePlayer::AddScore(uint score_add, COLLIDER_TYPE type)
{
	if (type == COLLIDER_TYPE::COLLIDER_PLAYER_SHOT) 
		players[0].score += score_add;
	else if (type == COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT)
		players[1].score += score_add;

}

void ModulePlayer::AddBonus(BONUS_TYPE type, Collider* col) {
	
	int player = 0;
	for (int i = 0; i < 2; ++i)
		if (players[i].player_collider == col) player = i;

	if (player == 0)
		AddScore(500, COLLIDER_PLAYER_SHOT);
	else
		AddScore(500, COLLIDER_PLAYER2_SHOT);

	if (type != players[player].current_bonus) {
		players[player].current_bonus = type;
		players[player].amount_bonus--;
		if (players[player].amount_bonus < 0)
			players[player].amount_bonus = 0;
	}
	else {
		if (players[player].amount_bonus < 3)
			++players[player].amount_bonus;
	}
}

void ModulePlayer::SpawnBits(bool player1)
{
	iPoint spawn_position;
	float factor = (float)M_PI / 180.0f;
	if (player1) {
		for (int i = 0; i < 360; i += 30) {
			spawn_position.x = (int)(players[0].player_world_x + SPRITE_WIDTH / 2 + SPRITE_WIDTH / 4 * cos(i * factor));
			spawn_position.y = (int)(players[0].player_y + App->render->camera.y + SPRITE_HEIGHT / 2 + SPRITE_HEIGHT / 4 * sin(i * factor));
			fPoint direction = { (float)spawn_position.x - (players[0].player_world_x + SPRITE_WIDTH / 2), (float)spawn_position.y - (players[0].player_y + App->render->camera.y + SPRITE_HEIGHT / 2) };
			App->particles->AddParticle(PLAYER_BITS, spawn_position.x, spawn_position.y, direction, true, 200);
		}

		for (int i = 15; i < 376; i += 32) {
			spawn_position.x = (int)(players[0].player_world_x + SPRITE_WIDTH / 2 + SPRITE_WIDTH / 4 * cos(i * factor));
			spawn_position.y = (int)(players[0].player_y + App->render->camera.y + SPRITE_HEIGHT / 2 + SPRITE_HEIGHT / 4 * sin(i * factor));
			fPoint direction = { (float)spawn_position.x - (players[0].player_world_x + SPRITE_WIDTH / 2), (float)spawn_position.y - (players[0].player_y + App->render->camera.y + SPRITE_HEIGHT / 2) };
			App->particles->AddParticle(PLAYER_BITS, spawn_position.x, spawn_position.y, direction, true, 400);
		}

		for (int i = 0; i < 360; i += 32) {
			spawn_position.x = (int)(players[0].player_world_x + SPRITE_WIDTH / 2 + SPRITE_WIDTH / 4 * cos(i * factor));
			spawn_position.y = (int)(players[0].player_y + App->render->camera.y + SPRITE_HEIGHT / 2 + SPRITE_HEIGHT / 4 * sin(i * factor));
			fPoint direction = { (float)spawn_position.x - (players[0].player_world_x + SPRITE_WIDTH / 2), (float)spawn_position.y - (players[0].player_y + App->render->camera.y + SPRITE_HEIGHT / 2) };
			App->particles->AddParticle(PLAYER_BITS, spawn_position.x, spawn_position.y, direction, true, 600);
		}

	}
	else {
		for (int i = 0; i < 360; i += 30) {
			spawn_position.x = (int)(players[1].player_world_x + SPRITE_WIDTH / 2 + SPRITE_WIDTH / 4 * cos(i * factor));
			spawn_position.y = (int)(players[1].player_y + App->render->camera.y + SPRITE_HEIGHT / 2 + SPRITE_HEIGHT / 4 * sin(i * factor));
			fPoint direction = { (float)spawn_position.x - (players[1].player_world_x + SPRITE_WIDTH / 2), (float)spawn_position.y - (players[1].player_y + App->render->camera.y + SPRITE_HEIGHT / 2) };
			App->particles->AddParticle(PLAYER_BITS, spawn_position.x, spawn_position.y, direction, false, 200);
		}

		for (int i = 15; i < 376; i += 32) {
			spawn_position.x = (int)(players[1].player_world_x + SPRITE_WIDTH / 2 + SPRITE_WIDTH / 4 * cos(i * factor));
			spawn_position.y = (int)(players[1].player_y + App->render->camera.y + SPRITE_HEIGHT / 2 + SPRITE_HEIGHT / 4 * sin(i * factor));
			fPoint direction = { (float)spawn_position.x - (players[1].player_world_x + SPRITE_WIDTH / 2), (float)spawn_position.y - (players[1].player_y + App->render->camera.y + SPRITE_HEIGHT / 2) };
			App->particles->AddParticle(PLAYER_BITS, spawn_position.x, spawn_position.y, direction, false, 400);
		}

		for (int i = 0; i < 360; i += 32) {
			spawn_position.x = (int)(players[1].player_world_x + SPRITE_WIDTH / 2 + SPRITE_WIDTH / 4 * cos(i * factor));
			spawn_position.y = (int)(players[1].player_y + App->render->camera.y + SPRITE_HEIGHT / 2 + SPRITE_HEIGHT / 4 * sin(i * factor));
			fPoint direction = { (float)spawn_position.x - (players[1].player_world_x + SPRITE_WIDTH / 2), (float)spawn_position.y - (players[1].player_y + App->render->camera.y + SPRITE_HEIGHT / 2) };
			App->particles->AddParticle(PLAYER_BITS, spawn_position.x, spawn_position.y, direction, false, 600);
		}
	}
}
