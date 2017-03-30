#ifndef __MODULESTAGE6_H__
#define __MODULESTAGE6_H__

#include "Module.h"
#include "Globals.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class ModuleStage6 : public Module {
public:

	ModuleStage6();
	~ModuleStage6();

	bool Start();
	update_status Update();
	bool CleanUp();

private:

	SDL_Texture* stage_background = nullptr;
	int background_pos;
	Mix_Music* music;
	Mix_Music* boss_music;
	Mix_Chunk* autoattack_fx;
};


#endif //__MODULESTAGE6_H__