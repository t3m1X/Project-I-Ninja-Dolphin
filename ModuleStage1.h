#ifndef _MODULESTAGE1_H
#define _MODULESTAGE1_H

#include "Module.h"
#include "Globals.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "p2Point.h"
#include "ModuleFonts.h"


struct SDL_Texture;

class ModuleStage1 : public Module
{
public:
	ModuleStage1();
	~ModuleStage1();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	void PrintWater(Animation* anim, int y_start, int n_tiles);

private:

	float cloud_position;

	SDL_Texture* stage_background = nullptr;
	SDL_Texture* water_texture = nullptr;
	SDL_Texture* background_animations = nullptr;
	Mix_Music* music;
	Animation sea_water;
	Animation river_water;
	Animation coast;
};
#endif
