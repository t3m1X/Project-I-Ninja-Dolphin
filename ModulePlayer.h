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
#define LASER_COOLDOWN 200

struct SDL_Texture;
struct Collider;
enum BONUS_TYPE;

enum player_state {
	IDLE = 0,
	LEFT,
	RIGHT,
	FORWARD,
	STOP

};

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
	void AddBonus(BONUS_TYPE type);

private:
	
	SDL_Texture* player;
	SDL_Rect player_sprite;
	SDL_Rect player_sprite_godmode;
	SDL_Rect shadow_idle;
	SDL_Rect shadow_left;
	SDL_Rect shadow_right;

	player_state state;
	Mix_Chunk* laser_sfx;
	uint sdl_clock;
	uint timing = 0;
	uint sdl_shot;

	Animation player_fire_forward;
	Animation player_fire_left;
	Animation player_fire_right;

	Animation player_left;
	Animation player_right;
	Animation player_left_godmode;
	Animation player_right_godmode;

	BONUS_TYPE current_bonus;
	int amount_bonus;
	
	Collider* player_collider;
	uint score = 0;
	uint highscore = 0;
	char* score_text;
	char* highscore_text;
	TTF_Font* font;

	int player_x, player_y;

	bool godmode = false;
};

#endif