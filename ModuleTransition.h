#ifndef __MODULETransition_H__
#define __MODULETransition_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Rect;
struct SDL_Texture;

class ModuleTransition : public Module
{
public:
	ModuleTransition();
	~ModuleTransition();

	bool Start();
	update_status Update();
	bool CleanUp();

	bool Transition(Module* module_off, Module* module_on, float time = 2.0f);

private:

	enum transition_step
	{
		none,
		transition_to_black,
		transition_from_black
	} current_step = transition_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	SDL_Texture* loading_screen;
	Animation loading_screen_animation;
	Animation loading_screen_animation_b;
	Module* to_disable;
	Module* to_enable;
};

#endif //__MODULETransition_H__
