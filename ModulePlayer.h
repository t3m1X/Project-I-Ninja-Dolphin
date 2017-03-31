#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL.h"

struct SDL_Texture;

class ModulePlayer : public Module {
public:

	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* player;
	SDL_Rect player_sprite;
	SDL_Rect player_sprite_left;
	SDL_Rect player_sprite_right;
	int player_x, player_y;

};

#endif