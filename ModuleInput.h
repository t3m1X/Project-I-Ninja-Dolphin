#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL_gamecontroller.h"

#define MAX_KEYS 300
#define MAX_CONTROLLERS 2


struct _SDL_Haptic;

enum KEY_STATE {
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum CONTROLLER_AXIS {
	LEFT_X = 0,
	LEFT_Y,
	RIGHT_X,
	RIGHT_Y,
	LEFT_TRIGGER,
	RIGHT_TRIGGER,
};

struct Controller {
	int index;
	SDL_GameController* controller;
	_SDL_Haptic* haptic;
	float intensity = 0;
	uint end_time = 0;

	KEY_STATE buttons[SDL_CONTROLLER_BUTTON_MAX];
	float axes[SDL_CONTROLLER_AXIS_MAX];

	Controller(SDL_GameController* _controller, _SDL_Haptic* _haptic) : controller(_controller), haptic(_haptic)
	{
		for (int i = 0; i < 6; ++i)
			axes[i] = 0;
	}

};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	KEY_STATE GetControllerButton(int player, SDL_GameControllerButton button);
	float GetControllerAxis(int player, SDL_GameControllerAxis axis);
	bool HasController(int player);
	void ShakeController(int player, int time, float strength = 0.5f);
	bool CleanUp();

public:
	KEY_STATE keyboard[MAX_KEYS];
	Controller* controllers[MAX_CONTROLLERS];
};

#endif // __ModuleInput_H__