#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Globals.h"
#include "ModuleAudio.h"
#include "SDL\include\SDL.h"
#include "ModuleCollision.h"
#include "ModuleFonts.h"
#include "Animation.h"
#include "p2Point.h"

#define PLAYER_SPEED 3
#define SPRITE_WIDTH 57
#define SPRITE_HEIGHT 49
#define SHADOW_WIDTH 29
#define SHADOW_HEIGHT 25
#define SHOT_COOLDOWN 400
#define LASER_COOLDOWN 20
#define BIG_LASER_COOLDOWN 30

struct SDL_Texture;
struct Collider;
enum BONUS_TYPE;

class ModulePlayer : public Module {
public:

	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	iPoint GetPos();
	void AddScore(uint score_add);
	void AddBonus(BONUS_TYPE type, Collider* col = nullptr);

private:
	enum player_state {
		OFF = 0,
		IDLE,
		LEFT,
		RIGHT,
		FORWARD,
		STOP
	};

	enum player_animation {
		AN_IDLE = 0,
		AN_IDLE_GOD,
		AN_LEFT,
		AN_LEFT_GOD,
		AN_RIGHT,
		AN_RIGHT_GOD,
		AN_FIRE,
		AN_FIRE_LEFT,
		AN_FIRE_RIGHT,
		AN_MAX
	};

	enum player_input {
		PI_FORWARD = 0,
		PI_BACK,
		PI_LEFT,
		PI_RIGHT,
		PI_SHOOT,
		PI_BOMB,
		PI_GODMODE,
		PI_MAX
	};
	
	struct player_struct{
		int player_x, player_y;
		player_state state;
		Collider* player_collider = nullptr;

		Animation animations[AN_MAX];
		SDL_Scancode inputs[PI_MAX];

		uint score = 0;
		bool godmode = false;

		BONUS_TYPE current_bonus;
		int amount_bonus = 0;

		uint sdl_shot = 0;

	};
	SDL_Texture* player;

	SDL_Rect shadow_idle;
	SDL_Rect shadow_left;
	SDL_Rect shadow_right;

	Mix_Chunk* laser_sfx;
	uint sdl_clock;

	uint highscore = 0;
	TTF_Font* font;

	player_struct players[2];
};

#endif