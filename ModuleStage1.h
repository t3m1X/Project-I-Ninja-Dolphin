#ifndef _MODULESTAGE1_H
#define _MODULESTAGE1_H

#include "Module.h"
#include "Globals.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"


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

	SDL_Texture* stage_background = nullptr;
	int background_pos;
	Mix_Music* music;
	

};
#endif
