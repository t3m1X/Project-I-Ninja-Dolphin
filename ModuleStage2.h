#ifndef _MODULESTAGE2_H
#define _MODULESTAGE2_H

#include "Module.h"
#include "Globals.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class ModuleStage2 : public Module
{
public:
	ModuleStage2();
	~ModuleStage2();

	bool Start();
	update_status Update();
	bool CleanUp();

private:

	SDL_Texture* stage_background = nullptr;
	int background_pos;
	Mix_Music* music;
	Mix_Music* boss_music;

};

#endif
